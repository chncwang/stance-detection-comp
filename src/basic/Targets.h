#ifndef SRC_BASIC_TARGETS_H
#define SRC_BASIC_TARGETS_H

#include <vector>
#include <string>
#include "MyLib.h"
#include "Example.h"

const std::vector<string> &getStanceTargets() {
	static std::vector<std::string> targets = {"Atheism", "Climate Change is a Real Concern", "Feminist Movement", "Hillary Clinton", "Legalization of Abortion", "Donald Trump"};
	return targets;
}

std::vector<vector<string> > getStanceTargetWordVectors() {
	using std::move;
	auto &targets = getStanceTargets();
	 std::vector<vector<string> > result;
		for (const std::string & str : targets) {
			vector<string> words;
                        split_bychar(str, words);
			result.push_back(move(words));
		}

	return result;
}

bool isTargetWordInTweet(const Feature &feature) {
	const std::string &firstWord = feature.m_target_words.at(0);
	std::vector<std::string> keywords;
	if (firstWord == "hillary") {
		keywords = {"hillary", "clinton"};
	}
	else if (firstWord == "donald") {
		keywords = {"donald", "trump"};
	}
	else if (firstWord == "atheism") {
		keywords = { "atheism", "atheist" };
	}
	else if (firstWord == "climate") {
		keywords = {"climate"};
	}
	else if (firstWord == "feminist") {
		keywords = {"feminism", "feminist"};
	}
	else if (firstWord == "legalization") {
		keywords = {"abortion", "aborting"};
	}
	else {
		abort();
	}
	for (const std::string &keyword : keywords) {
        for (const std::string &tweetword : feature.m_tweet_words) {
            if (tweetword.find(keyword) != std::string::npos) {
                return true;
            }
        }
	}

	return false;
}

#endif // !TARGETS_H
