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
				instance->m_target_words = {"#atheism"};
                                instance->m_target_tfidf_words = {"atheist", "#atheism", "atheism", "#atheist", "god", "religion", "@atheist_roo", "#god", "#jesus", "@atheist_eh", "#religion", "believe", "dawkins", "#atheists", "clock", "#bible", "@atheist_dh", "november", "am", "richard", "religious", "islam", "about", "atheists", "at", "can", "all", "how", "someone", "really", "from", "we", "@cenkuygur", "like", "#islam", "@rickygervais", "people", "there", "christian", "faith", "@samharrisorg", "because", "boy", "just", "@skepticnikki", "quran", "harris", "compares", "man", "know", "say", "one", "@atheist_krishna", "isis", "when", "out", "@atxbantamathy", "jesus", "think", "should", "@atheist_bot", "only", "@godlessutopia", "@oldrifleman", "did", "#christ", "sam", "up", "true", "#love", "were", "here", "more", "@egy_atheist", "#lord", "new", "evidence", "belief", "muslim", "his", "good", "then", "even", "@richarddawkins", "ahmed", "read", "everyone", "time", "@ryelwilliams", "thanksgiving", "tell", "yes", "muslims", "being", "ricky", "@amadeusalmighty", "gervais", "after", "militant", "values"};
			}
			else if (firstWord == "Climate") {
				instance->m_target_words = {"#climatechange"};
                                instance->m_target_tfidf_words = {"climate", "change", "#nowph", "paris", "support", "po", "campaign", "#cop21", "@r_faulkerson", "action", "#climate", "one", "ambassadors", "summit", "help", "youth", "take", "talks", "at", "science", "@aldenrichards02", "call", "aldub", "#climatechange", "nation", "world", "us", "let", "re", "natin", "we", "pa", "yung", "trend", "naman", "global", "our", "conference", "@allscienceglobe", "awareness", "about", "can", "obama", "rebuke", "all", "development", "isis", "climate-friendly", "recognized", "rice", "best", "from", "alden", "more", "how", "terrorists", "@nowph_org", "new", "bae", "un", "htt…", "leaders", "powerful", "chage", "join", "nov", "#actonclimate", "plan", "scientists", "report", "energy", "march", "national", "snow", "says", "deal", "government", "precip", "meet", "study", "great", "blog", "exxon", "day", "now", "record", "greg", "part", "fight", "agreement", "africa", "up", "@ofctrendsetter", "need", "india", "than", "children", "could", "books", "here"};
			}
			else if (firstWord == "Feminist") {
				instance->m_target_words = { "#feminism" };
                                instance->m_target_tfidf_words = {"feminist", "feminism", "jumpman", "woman", "jump", "disgusting", "jumpma-", "jumptrans", "men", "wrong", "need", "@feministculture", "too", "women", "#feminism", "bread", "yeast", "about", "so…", "vagina", "makes", "blogger", "@blackpplvines", "s…", "own", "@lordflaconegro", "from", "documentary", "charli", "boldly", "xcx", "vegan", "white", "just", "gender", "all", "like", "because", "we", "out", "equality", "music", "rape", "being", "intersectional", "feminists", "@woridstarhiph0p", "@shujarabbani", "when", "issue", "industry", "#feminist", "diversity", "mc", "ruth_blane", "most", "against", "how", "think", "officer", "watch", "until", "#heforshe", "sourdough", "people", "everything", "twitter", "love", "muslim", "man", "#16days", "@charli_xcx", "benefits", "sends", "inequality", "than", "fetty", "f-word", "wap", "better", "@byetyler", "caitlyn", "jenner", "gay…", "found", "actually", "geller", "threat", "make", "now", "pamela", "@un", "girl", "misuse", "u", "some", "later", "up", "@worldstarc0medy", "at"};
			}
			else if (firstWord == "Hillary") {
				instance->m_target_words = {"#hillaryclinton"};
                                instance->m_target_tfidf_words = {"hillary", "clinton", "@hillaryclinton", "sanders", "cnn", "emails", "bernie", "reporter", "trump", "illegal", "paul", "rand", "aide", "iowa", "bill", "#hillary", "at", "poll", "over", "lies", "immigrants", "smear", "#benghazi", "@berniesanders", "benghazi", "how", "#hillaryclinton", "from", "campaign", "#hillary2016", "show", "coordinated", "like", "about", "says", "leaked", "#uniteblue", "women", "#hillaryforprison2016", "than", "vote", "more", "attack", "just", "out", "suspended", "video", "fundraiser", "against", "we", "right", "tonight", "worked", "@realdonaldtrump", "speaking", "term", "new", "firm", "#tcot", "did", "hearing", "all", "up", "reveal", "mason", "people", "during", "philadelphia", "gop", "obama", "investment", "news", "fired", "saying", "team", "city", "now", "unethical", "after", "behavior", "ibd", "get", "target", "#pjnet", "state", "lead", "should", "make", "@randpaul", "clinton’s", "want", "#clinton", "access", "#feelthebern", "health", "when", "president", "can", "policy", "politicians"};
			}
			else if (firstWord == "Legalization") {
				instance->m_target_words = {"#prochoice"};
                                instance->m_target_tfidf_words = {"abortion", "#abortion", "#thanksgivingclapback", "gay", "law", "third", "pass", "potatoes", "mashed", "@kareemdant", "choice", "wisconsin", "being", "please", "anti-abortion", "abortions", "judge", "clinic", "parenthood", "#prolife", "planned", "down", "court", "@lifenewshq", "should", "women", "about", "after", "life", "finesses", "scandal", "stance", "rubio", "having", "criticized", "baby", "we", "now", "when", "activist", "providers", "silent", "unconstitutional", "rights", "strikes", "just", "had", "democrats", "doctors", "laws", "at", "negative", "listen", "plate", "appointed", "ronald", "night", "reagan", "pro-life", "killing", "cruz", "did", "effects", "clinics", "babies", "#prochoice", "like", "against", "dealing", "their", "birth", "@ppact", "ted", "restrictions", "one", "provider", "missouri", "get", "surrogate", "murder", "demands", "@lilagracerose", "triplets", "people", "legal", "want", "#defundpp", "4th", "never", "abc", "out", "pregnant", "ireland", "how", "only", "aunt", "woman", "privileges", "showing", "#best"};
			}
			else if (firstWord == "Donald") {
				instance->m_target_words = { "#donaldtrump" };
                                instance->m_target_tfidf_words = {"trump", "donald", "@realdonaldtrump", "hillary", "clinton", "cruz", "#trump", "mcdonald", "laquan", "shooting", "obama", "rubio", "#trump2016", "fox", "debate", "video", "says", "like", "wins", "@hillaryclinton", "president", "his", "vote", "over", "palin", "statement", "from", "gop", "run", "another", "sanders", "out", "calls", "at", "jeb", "we", "#laquanmcdonald", "watch", "kasich", "@anncoulter", "cnn", "other", "him", "than", "how", "win", "can", "just", "laughing", "country", "trump’s", "against", "get", "about", "said", "anyone", "america", "news", "poll", "one", "beat", "think", "immigration", "all", "muslim", "up", "hate", "justified", "@sarahkendzior", "republican", "hospitals", "@danscavino", "expert", "now", "most", "man", "c…", "posts", "bernie", "christie", "bombing", "when", "people", "tacos", "corruption", "more", "week", "ad", "sarah", "benghazi", "sc", "event", "only", "us", "vs", "new", "polls", "great", "want", "nevada"};
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
