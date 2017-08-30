#ifndef SRC_ComputionGraph_H_
#define SRC_ComputionGraph_H_

#include "ModelParams.h"
#include "Utf.h"
#include "LSTM1.h"
#include "MyLib.h"


// Each model consists of two parts, building neural graph and defining output losses.
class GraphBuilder {
public:
    std::vector<LookupNode> _tweet_nodes;
    std::vector<LookupNode> _target_nodes;
    std::vector<LookupNode> _target_tfidf_nodes;
    UniNode _neural_output;
    LSTM1Builder _left_to_right_tweet_lstm;
    LSTM1Builder _right_to_left_tweet_lstm;
    LSTM1Builder _left_to_right_target_lstm;
    LSTM1Builder _right_to_left_target_lstm;
    std::vector<ConcatNode> _tweet_lstm_nodes;
    std::vector<ConcatNode> _target_lstm_nodes;
    AvgPoolNode _tfidf_pool;
    MaxPoolNode _target_pool;
    ConcatNode _pool_concat_node;
    AttentionBuilder _attention_builder;

    Graph *_graph;
    ModelParams *_modelParams;
    const static int max_sentence_length = 1024;

public:
    //allocate enough nodes
    void createNodes(int length_upper_bound) {
        _target_nodes.resize(length_upper_bound);
        _tweet_nodes.resize(length_upper_bound);
        _left_to_right_tweet_lstm.resize(length_upper_bound);
        _right_to_left_tweet_lstm.resize(length_upper_bound);
        _left_to_right_target_lstm.resize(length_upper_bound);
        _right_to_left_target_lstm.resize(length_upper_bound);
        _target_tfidf_nodes.resize(length_upper_bound);
        _tweet_lstm_nodes.resize(length_upper_bound);
        _target_lstm_nodes.resize(length_upper_bound);
        _attention_builder.resize(length_upper_bound);
        _target_pool.setParam(length_upper_bound);
        _tfidf_pool.setParam(length_upper_bound);
    }

public:
    void initial(Graph *pcg, ModelParams &model, HyperParams &opts) {
        _graph = pcg;
        for (auto &n : _target_nodes) {
            n.init(opts.wordDim, opts.dropProb);
            n.setParam(&model.words);
        }
        for (LookupNode &n : _tweet_nodes) {
            n.init(opts.wordDim, opts.dropProb);
            n.setParam(&model.words);
        }

        for (auto &n : _target_tfidf_nodes) {
            n.init(opts.wordDim, opts.dropProb);
            n.setParam(&model.words);
        }
        _left_to_right_target_lstm.init(&model.target_left_to_right_lstm_params, opts.dropProb, true);
        _right_to_left_target_lstm.init(&model.target_right_to_left_lstm_params, opts.dropProb, false);
        _left_to_right_tweet_lstm.init(&model.tweet_left_to_right_lstm_params, opts.dropProb, true);
        _right_to_left_tweet_lstm.init(&model.tweet_right_to_left_lstm_params, opts.dropProb, false);

        for (auto &n : _tweet_lstm_nodes) {
            n.init(opts.hiddenSize * 2, -1);
        }

        for (auto &n : _target_lstm_nodes) {
            n.init(opts.hiddenSize * 2, -1);
        }

        _target_pool.init(opts.hiddenSize * 2, -1);
        _tfidf_pool.init(opts.wordDim, -1);
        _pool_concat_node.init(opts.hiddenSize * 2 + opts.wordDim, -1);
        _attention_builder.init(&model._attention_params);

        _neural_output.init(opts.labelSize, -1);
        _neural_output.setParam(&model.olayer_linear);
        _modelParams = &model;
    }

public:
    // some nodes may behave different during training and decode, for example, dropout
    inline void forward(const Feature &feature, bool bTrain = false) {
        _graph->train = bTrain;

        for (int i = 0; i < feature.m_target_words.size(); ++i) {
            _target_nodes.at(i).forward(_graph, feature.m_target_words.at(i));
        }
        std::vector<PNode> target_nodes_ptrs = toPointers<LookupNode, Node>(_target_nodes, feature.m_target_words.size());
        _left_to_right_target_lstm.forward(_graph, target_nodes_ptrs);
        _right_to_left_target_lstm.forward(_graph, target_nodes_ptrs);
        for (int i = 0; i < feature.m_target_words.size(); ++i) {
            _target_lstm_nodes.at(i).forward(_graph, &_left_to_right_target_lstm._hiddens.at(i), &_right_to_left_target_lstm._hiddens.at(i));
        } std::vector<PNode> target_lstm_ptrs = toPointers<ConcatNode, Node>(_target_lstm_nodes, feature.m_target_words.size());
        _target_pool.forward(_graph, target_lstm_ptrs);

        for (int i = 0; i < feature.m_target_tfidf_words->size(); ++i) {
            _target_tfidf_nodes.at(i).forward(_graph, feature.m_target_tfidf_words->at(i));
        }

        _tfidf_pool.forward(_graph, toPointers<LookupNode, Node>(_target_tfidf_nodes, feature.m_target_tfidf_words->size()));
        _pool_concat_node.forward(_graph, &_tfidf_pool, &_target_pool);

        for (int i = 0; i < feature.m_tweet_words.size(); ++i) {
            _tweet_nodes.at(i).forward(_graph, feature.m_tweet_words.at(i));
        }
        std::vector<PNode> tweet_nodes_ptrs = toPointers<LookupNode, Node>(_tweet_nodes, feature.m_tweet_words.size());

        _left_to_right_tweet_lstm.forward(_graph, tweet_nodes_ptrs);
        _right_to_left_tweet_lstm.forward(_graph, tweet_nodes_ptrs);

        for (int i = 0; i < feature.m_tweet_words.size(); ++i) {
            _tweet_lstm_nodes.at(i).forward(_graph, &_left_to_right_tweet_lstm._hiddens.at(i), &_right_to_left_tweet_lstm._hiddens.at(i));
        }

        std::vector<PNode> lstm_ptrs = toPointers<ConcatNode, Node>(_tweet_lstm_nodes, feature.m_tweet_words.size());
        _attention_builder.forward(_graph, lstm_ptrs, &_pool_concat_node);

        _neural_output.forward(_graph, &_attention_builder._hidden);
    }
};


#endif /* SRC_ComputionGraph_H_ */
