#ifndef _JST_READER_
#define _JST_READER_

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
using namespace std;

#include "Instance.h"
#include "Targets.h"

class Reader
{
public:
	Reader()
	{
	}

	virtual ~Reader()
	{
		if (m_inf.is_open()) m_inf.close();
	}
	int startReading(const char *filename) {
		if (m_inf.is_open()) {
			m_inf.close();
			m_inf.clear();
		}
		m_inf.open(filename);

    if (!m_inf.is_open()) {
			cout << "Reader::startReading() open file err: " << filename << endl;
			return -1;
		}

		return 0;
	}

	void finishReading() {
		if (m_inf.is_open()) {
			m_inf.close();
			m_inf.clear();
		}
	}
	virtual Instance *getNext() = 0;
protected:
	ifstream m_inf;

	int m_numInstance;

	Instance m_instance;
};

vector<string> readLines(const string &fullFileName) {
	vector<string> lines;
	std::ifstream input(fullFileName);
	for (std::string line; getline(input, line);) {
		lines.push_back(line);
	}
	return lines;
}

void readLineToInstance(const string &line, Instance *instance) {
	//cout << "Reader readLineToInstance line:" << line << endl;
	int tailIndex = -1;
	int i = 0;
	auto targetWordVectors = getStanceTargetWordVectors();
	for (const string &target : getStanceTargets()) {
		string::size_type index = line.find(target);
		if (index <= 8) {
			string firstWord = targetWordVectors.at(i).at(0);
			if (firstWord == "Atheism") {
				instance->m_target_words = {"atheist"};
                                instance->m_target_tfidf_words = {"atheist", "#atheism", "atheism", "#atheist", "an", "i", "god", "you", "is", "a", "the", "n't", "to", "religion", "and", "it", "of", "that", "@atheist_roo", "#god", "in", "#jesus", "@atheist_eh", "do", "'m", "are", "be", "not", "if", "but", "#religion", "for", "believe", "'s", "my", "dawkins", "does", "rt", "no", "he", "they", "#atheists", "who", "clock", "#bible", "@atheist_dh", "this", "have", "your", "was", "november", "am", "richard", "religious", "me", "islam", "about", "on", "so", "what", "atheists", "as", "with", "has", "&amp;", "at", "can", "all", "how", "someone", "really", "from", "we", "@cenkuygur", "like", "or", "by", "#islam", "@rickygervais", "people", "there", "christian", "faith", "@samharrisorg", "because", "boy", "just", "@skepticnikki", "quran", "'re", "harris", "compares", "man", "would", "know", "say", "one", "@atheist_krishna", "isis", "when"};
			}
			else if (firstWord == "Climate") {
				instance->m_target_words = {"climate", "change", "is", "a", "real", "concern"};
                                instance->m_target_tfidf_words = {"climate", "change", "the", "to", "of", "#nowph", "a", "on", "paris", "in", "support", "rt", "po", "for", "campaign", "is", "#cop21", "@r_faulkerson", "action", "#climate", "'s", "as", "one", "ambassadors", "summit", "and", "help", "will", "youth", "take", "talks", "at", "science", "be", "@aldenrichards02", "call", "aldub", "#climatechange", "nation", "world", "us", "let", "re", "&amp;", "natin", "we", "pa", "yung", "trend", "naman", "global", "via", "our", "by", "conference", "@allscienceglobe", "awareness", "this", "it", "about", "can", "are", "obama", "rebuke", "all", "development", "what", "isis", "that", "climate-friendly", "recognized", "rice", "best", "from", "do", "alden", "more", "how", "terrorists", "has", "@nowph_org", "new", "you", "bae", "un", "i", "htt…", "leaders", "powerful", "chage", "n't", "join", "with", "your", "nov", "#actonclimate", "plan", "scientists", "report", "energy"};
			}
			else if (firstWord == "Feminist") {
				instance->m_target_words = { "feminist", "movement" };
                                instance->m_target_tfidf_words = {"feminist", "feminism", "jumpman", "'re", "you", "and", "me", "why", "woman", "with", "not", "'s", "jump", "disgusting", "jumpma-", "what", "jumptrans", "men", "or", "wrong", "i", "a", "is", "need", "rt", "@feministculture", "too", "women", "#feminism", "the", "bread", "her", "yeast", "it", "n't", "that", "about", "so…", "vagina", "in", "to", "of", "makes", "blogger", "are", "this", "@blackpplvines", "for", "do", "s…", "own", "@lordflaconegro", "from", "documentary", "charli", "my", "boldly", "xcx", "vegan", "she", "white", "just", "on", "be", "but", "gender", "all", "if", "was", "who", "like", "because", "we", "out", "an", "'m", "equality", "music", "rape", "have", "&amp;", "being", "so", "as", "they", "intersectional", "feminists", "@woridstarhiph0p", "@shujarabbani", "when", "issue", "industry", "#feminist", "diversity", "mc", "ruth_blane", "most", "against", "your", "how"};
			}
			else if (firstWord == "Hillary") {
				instance->m_target_words = {"hillary", "clinton"};
                                instance->m_target_tfidf_words = {"hillary", "clinton", "to", "&gt;", "in", "the", "for", "'s", "rt", "of", "with", "is", "a", "@hillaryclinton", "sanders", "and", "cnn", "emails", "i", "her", "bernie", "she", "&amp;", "reporter", "trump", "on", "illegal", "paul", "rand", "aide", "it", "that", "you", "iowa", "have", "not", "bill", "via", "has", "#hillary", "was", "n't", "will", "no", "be", "are", "at", "do", "poll", "over", "this", "lies", "immigrants", "if", "but", "smear", "#benghazi", "@berniesanders", "benghazi", "how", "#hillaryclinton", "from", "campaign", "as", "#hillary2016", "show", "coordinated", "like", "about", "says", "leaked", "#uniteblue", "by", "they", "women", "why", "who", "#hillaryforprison2016", "he", "than", "vote", "more", "attack", "just", "out", "suspended", "would", "video", "fundraiser", "against", "we", "right", "so", "tonight", "worked", "@realdonaldtrump", "speaking", "term", "new", "firm"};
			}
			else if (firstWord == "Legalization") {
				instance->m_target_words = {"legalization", "of", "abortion"};
                                instance->m_target_tfidf_words = {"abortion", "#abortion", "an", "is", "your", "#thanksgivingclapback", "a", "the", "gay", "law", "third", "you", "pass", "potatoes", "mashed", "@kareemdant", "was", "choice", "wisconsin", "of", "to", "being", "so", "please", "rt", "for", "'s", "on", "anti-abortion", "abortions", "in", "n't", "that", "judge", "who", "i", "and", "she", "it", "by", "clinic", "parenthood", "have", "are", "#prolife", "planned", "down", "court", "@lifenewshq", "should", "women", "not", "&amp;", "with", "about", "be", "after", "this", "life", "finesses", "scandal", "stance", "rubio", "having", "criticized", "baby", "we", "now", "when", "activist", "providers", "her", "silent", "do", "unconstitutional", "if", "rights", "me", "strikes", "but", "just", "as", "had", "democrats", "doctors", "laws", "at", "negative", "listen", "plate", "appointed", "ronald", "via", "night", "reagan", "pro-life", "killing", "cruz", "did", "effects"};
			}
			else if (firstWord == "Donald") {
				instance->m_target_words = { "donald", "trump" };
                                instance->m_target_tfidf_words = {"trump", "donald", "@realdonaldtrump", "hillary", "clinton", "the", "to", "is", "and", "a", "of", "for", "he", "i", "on", "rt", "'s", "in", "you", "if", "n't", "will", "cruz", "#trump", "it", "be", "mcdonald", "laquan", "this", "&amp;", "shooting", "obama", "do", "not", "rubio", "#trump2016", "but", "fox", "debate", "would", "that", "video", "says", "like", "so", "wins", "@hillaryclinton", "have", "president", "his", "does", "vote", "or", "has", "over", "palin", "she", "statement", "with", "from", "who", "are", "gop", "run", "another", "sanders", "out", "calls", "at", "jeb", "no", "we", "#laquanmcdonald", "watch", "kasich", "@anncoulter", "cnn", "other", "him", "as", "than", "they", "how", "win", "can", "just", "laughing", "country", "trump’s", "against", "get", "about", "said", "anyone", "america", "news", "poll", "one", "what", "was"};
			}
			else {
                          std::cout <<firstWord << std::endl;
				abort();
			}

			tailIndex = index + target.size();
			//cout << "Reader readLineToInstance tailIndex:" << tailIndex << endl;
			break;
		}
		++i;
	}

	if (tailIndex == -1) {
		cout << "target not found!" << line << endl;
		assert(false);
	}

	string::size_type index = string::npos;
	for (int i = 0; i < 3; ++i) {
		Stance stance = static_cast<Stance>(i);
		const string &stanceStr = StanceToString(stance);
//                std::cout << "stanceStr:" << stanceStr <<std::endl;
		std::regex regex(stanceStr + "\r?$");
		for (auto it = std::sregex_iterator(line.begin(), line.end(), regex);
			it != std::sregex_iterator();
			++it)
		{
			index = it->position();
			instance->m_stance = stance;
			break;
		}
	}
        if (index == string::npos) {
          std::cout<< line << std::endl;
          abort();
        }

	assert(index != string::npos);

	string substring = line.substr(tailIndex, index - tailIndex);
	//std::cout << "Reader readLineToInstance substring:" << substring << endl;
	
	//std::regex regex("[\s\t]+(.+)");
	//std::smatch matcher;
	//if (!std::regex_search(substring, matcher, regex)) {
	//	//std::cout << "Reader readLineToInstance regex not found!" << endl;
	//	std::cout << "substring:" << substring << std::endl;
	//	assert(false);
	//}

	//string sentence = matcher.format("$1");
	//std::cout << "Reader readLineToInstance sentence:" << sentence << "|||" << endl;

	vector<string> rawwords;
    split_bychar(substring, rawwords);
	vector<string> words;
	for (string & rawword : rawwords) {
		if (rawword.empty()) continue;
		string word = normalize_to_lowerwithdigit(rawword);
		if (word == "rt" || word == "via" || word == "#semst") continue;
		//if (word == "thats" || word == "im" || word == "'s") continue;
		if (isPunctuation(word)) continue;

		std::string http = "http";
		if (!word.compare(0, http.size(), http)) {
			continue;
		}

		words.push_back(word);
	}

	assert(!words.empty());

	//std::cout << instance->m_stance << std::endl;
	instance->m_tweet_words = move(words);
}

vector<Instance> readInstancesFromFile(const string &fullFileName) {
	vector<string> lines = readLines(fullFileName);

	vector<Instance> instances;
	using std::move;
	for (int i = 0; i < lines.size(); ++i) {
		if (lines.at(i) == "ID Target Tweet Stance") continue;
		Instance ins;
		readLineToInstance(lines.at(i), &ins);
		instances.push_back(move(ins));
	}

	return instances;
}

#endif
