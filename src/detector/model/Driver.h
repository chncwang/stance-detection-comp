/*
* Driver.h
*
*  Created on: Mar 18, 2015
*      Author: mszhang
*/

#ifndef SRC_Driver_H_
#define SRC_Driver_H_

#include <iostream>
#include "ComputionGraph.h"
#include "Stance.h"
#include "MySoftMaxLoss.h"
#include "Targets.h"

//A native neural network classfier using only word embeddings

class Driver {
public:
    Driver(int memsize) {}

    ~Driver() {

    }

public:
    Graph _cg;  // build neural graphs
    vector<GraphBuilder> _builders;
    ModelParams _modelparams;  // model parameters
    HyperParams _hyperparams;

    Metric _favor_metric;
    Metric _against_metric;
    Metric _neural_metric;
    CheckGrad _checkgrad;
    ModelUpdate _ada;  // model update

public:
    //embeddings are initialized before this separately.
    void initial() {
        if (!_hyperparams.bValid()) {
            std::cout << "hyper parameter initialization Error, Please check!"
                << std::endl;
            abort();
        }
        if (!_modelparams.initial(_hyperparams)) {
            std::cout << "model parameter initialization Error, Please check!"
                << std::endl;
            abort();
        }
        _modelparams.exportModelParams(_ada);
        _modelparams.exportCheckGradParams(_checkgrad);

        _hyperparams.print();

        _builders.resize(_hyperparams.batch);

        for (int idx = 0; idx < _hyperparams.batch; idx++) {
            _builders[idx].createNodes(GraphBuilder::max_sentence_length);
            _builders[idx].initial(&_cg, _modelparams, _hyperparams);
        }

        setUpdateParameters(_hyperparams.nnRegular, _hyperparams.adaAlpha,
            _hyperparams.adaEps);
    }


    inline void TestInitial() {
        if (!_hyperparams.bValid()) {
            std::cout << "hyper parameter initialization Error, Please check!"
                << std::endl;
            return;
        }
        if (!_modelparams.TestInitial(_hyperparams)) {
            std::cout << "model parameter initialization Error, Please check!"
                << std::endl;
            return;
        }
        _modelparams.exportModelParams(_ada);
        _modelparams.exportCheckGradParams(_checkgrad);

        _hyperparams.print();

        _builders.resize(_hyperparams.batch);

        for (int idx = 0; idx < _hyperparams.batch; idx++) {
            _builders[idx].createNodes(GraphBuilder::max_sentence_length);
            _builders[idx].initial(&_cg, _modelparams, _hyperparams);
        }

        setUpdateParameters(_hyperparams.nnRegular, _hyperparams.adaAlpha,
            _hyperparams.adaEps);
    }


    inline dtype train(const vector<Example> &examples, int iter) {
        resetEval();
        _cg.clearValue();
        int example_num = examples.size();
        if (example_num > _builders.size()) {
            std::cout << "Driver train - input example number larger than predefined batch number example_num:" << example_num
                << " _builders.size():" << _builders.size() << std::endl;
            abort();
        }

        dtype cost = 0.0;

        for (int count = 0; count < example_num; count++) {
            const Example &example = examples[count];

            //forward
            _builders[count].forward(example.m_feature, true);

        }
        _cg.compute();

        for (int count = 0; count < example_num; count++) {
            const Example &example = examples[count];
            cost += _modelparams.loss.loss(&_builders[count]._neural_output,
                example.m_stance, _favor_metric, _against_metric, _neural_metric, example_num);
        }
        _cg.backward();

        return cost;
    }

    inline void predict(const Feature &feature, Stance &result) {
        _cg.clearValue();
        _builders[0].forward(feature);
        _cg.compute();

        int intResult;
        _modelparams.loss.predict(&_builders[0]._neural_output, intResult);
        result = static_cast<Stance>(intResult);
    }

    inline dtype cost(const Example &example) {
        _cg.clearValue();
        _builders[0].forward(example.m_feature, true);
        _cg.compute();

        dtype cost = _modelparams.loss.cost(&_builders[0]._neural_output,
            example.m_stance, 1);

        return cost;
    }


    void updateModel() {
        //_ada.update();
        _ada.updateAdam(10);
    }

    void checkgrad(const vector<Example> &examples, int iter) {
        ostringstream out;
        out << "Iteration: " << iter;
        _checkgrad.check(this, examples, out.str());
    }

private:
    inline void resetEval() {
        _favor_metric.reset();
        _against_metric.reset();
        _neural_metric.reset();
    }


    inline void
        setUpdateParameters(dtype nnRegular, dtype adaAlpha, dtype adaEps) {
        _ada._alpha = adaAlpha;
        _ada._eps = adaEps;
        _ada._reg = nnRegular;
    }

};

#endif /* SRC_Driver_H_ */
