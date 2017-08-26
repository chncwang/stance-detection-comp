#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "Stance.h"
#include <algorithm>

using namespace std;

class Feature
{
    public:
        vector<std::string> m_tweet_words;
        vector<std::string> m_target_words;
        vector<std::string> m_sparse_feats;
    public:
        void clear()
        {
            m_tweet_words.clear();
            m_sparse_feats.clear();
            m_target_words.clear();
        }
};

class Example
{
    public:
        Feature m_feature;
        Stance m_stance;
        //vector<dtype> m_label; //TODO

        void clear()
        {
            m_feature.clear();
        }
};

vector<int> getClassBalancedIndexes(const std::vector<Example> &examples, const std::array<float, 3> &ratios) {
    if (isEqual(ratios.at(0), 0)) {
        assert(isEqual(ratios.at(1), 0) && isEqual(ratios.at(2), 0));
        std::vector<int> indexes;
        for (int i = 0; i< examples.size(); ++i) {
            indexes.push_back(i);
        }
        return indexes;
    }

    std::array<std::vector<int>, 3> classSpecifiedIndexesArr;
    for (int i = 0; i < examples.size(); ++i) {
        const Example &example = examples.at(i);
        classSpecifiedIndexesArr.at(example.m_stance).push_back(i);
    }

    for (auto &v : classSpecifiedIndexesArr) {
        std::random_shuffle(v.begin(), v.end());
    }

    std::array<int, 3> counters = {static_cast<int>(classSpecifiedIndexesArr.at(0).size() / ratios.at(0)), static_cast<int>(classSpecifiedIndexesArr.at(1).size() / ratios.at(1)), static_cast<int>(classSpecifiedIndexesArr.at(2).size() / ratios.at(2)) };

    int minCounter = *std::min_element(counters.begin(), counters.end());
    std::vector<int> indexes;

    int x = 0;
    for (auto & v : classSpecifiedIndexesArr) {
        for (int i = 0; i < minCounter * ratios.at(x); ++i) {
            indexes.push_back(v.at(i));
        }
        x++;
    }

    std::random_shuffle(indexes.begin(), indexes.end());
    assert(indexes.size() == 3 * minCounter);
    return indexes;
}

#endif /*_EXAMPLE_H_*/
