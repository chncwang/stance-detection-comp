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
    const vector<std::string> * m_target_tfidf_words;
};

class Example
{
public:
	Feature m_feature;
	Stance m_stance;
};

vector<int> getClassBalancedIndexes(const std::vector<Example> &examples) {
	std::array<std::vector<int>, 3> classSpecifiedIndexesArr;
	for (int i = 0; i < examples.size(); ++i) {
		const Example &example = examples.at(i);
		classSpecifiedIndexesArr.at(example.m_stance).push_back(i);
	}

	for (auto &v : classSpecifiedIndexesArr) {
		std::random_shuffle(v.begin(), v.end());
	}

	std::array<int, 3> counters = {static_cast<int>(classSpecifiedIndexesArr.at(0).size()), static_cast<int>(classSpecifiedIndexesArr.at(1).size()), static_cast<int>(classSpecifiedIndexesArr.at(2).size()) };

	int minCounter = *std::min_element(counters.begin(), counters.end());
	std::vector<int> indexes;

	for (auto & v : classSpecifiedIndexesArr) {
		for (int i = 0; i < minCounter; ++i) {
			indexes.push_back(v.at(i));
		}
	}

	std::random_shuffle(indexes.begin(), indexes.end());
	assert(indexes.size() == 3 * minCounter);
	return indexes;
}

#endif /*_EXAMPLE_H_*/
