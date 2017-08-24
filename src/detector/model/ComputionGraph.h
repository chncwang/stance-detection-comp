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
  UniNode _neural_output;
  LSTM1Builder _left_to_right_tweet_lstm;
  LSTM1Builder _right_to_left_tweet_lstm;
  std::vector<ConcatNode> _lstm_nodes;
  AvgPoolNode _avg_pool;

  Graph *_graph;
  ModelParams *_modelParams;
  const static int max_sentence_length = 1024;

public:
  //allocate enough nodes
  void createNodes(int length_upper_bound) {
    _tweet_nodes.resize(length_upper_bound);
    _left_to_right_tweet_lstm.resize(length_upper_bound);
    _right_to_left_tweet_lstm.resize(length_upper_bound);
    _lstm_nodes.resize(length_upper_bound);
    _avg_pool.setParam(length_upper_bound);
  }

public:
  void initial(Graph *pcg, ModelParams &model, HyperParams &opts) {
    _graph = pcg;
    for (LookupNode &n : _tweet_nodes) {
      n.init(opts.wordDim, opts.dropProb);
      n.setParam(&model.words);
    }
    _left_to_right_tweet_lstm.init(&model.tweet_left_to_right_lstm_params, opts.dropProb, true);
    _right_to_left_tweet_lstm.init(&model.tweet_right_to_left_lstm_params, opts.dropProb, false);

    for (auto &n : _lstm_nodes) {
        n.init(opts.hiddenSize * 2, -1);
    }

    _avg_pool.init(opts.hiddenSize * 2, -1);

    _neural_output.init(opts.labelSize, -1);
    _neural_output.setParam(&model.olayer_linear);
    _modelParams = &model;
  }

public:
  // some nodes may behave different during training and decode, for example, dropout
  inline void forward(const Feature &feature, bool bTrain = false) {
    _graph->train = bTrain;

    for (int i = 0; i < feature.m_tweet_words.size(); ++i) {
      _tweet_nodes.at(i).forward(_graph, feature.m_tweet_words.at(i));
    }

    std::vector<PNode> tweet_nodes_ptrs = toPointers<LookupNode, Node>(_tweet_nodes, feature.m_tweet_words.size());

    _left_to_right_tweet_lstm.forward(_graph, tweet_nodes_ptrs);
    _right_to_left_tweet_lstm.forward(_graph, tweet_nodes_ptrs);

    for (int i = 0; i < feature.m_tweet_words.size(); ++i) {
        _lstm_nodes.at(i).forward(_graph, &_left_to_right_tweet_lstm._hiddens.at(i), &_right_to_left_tweet_lstm._hiddens.at(i));
    }

    std::vector<PNode> lstm_ptrs = toPointers<ConcatNode, Node>(_lstm_nodes, feature.m_tweet_words.size());

    _avg_pool.forward(_graph, lstm_ptrs);
    _neural_output.forward(_graph, &_avg_pool);
  }
};


#endif /* SRC_ComputionGraph_H_ */
