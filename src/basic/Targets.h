#ifndef SRC_BASIC_TARGETS_H
#define SRC_BASIC_TARGETS_H

#include <vector>
#include <array>
#include <string>
#include "MyLib.h"
#include "Example.h"

enum Target {
    ATHEISM = 0,
    CLIMATE_CHANGE = 1,
    FEMINIST_MOVEMENT = 2,
    HILLARY_CLINTON = 3,
    LEGALIZATION_OF_ABORTION = 4,
    DONALD_TRUMP = 5
};

const std::vector<string> &getStanceTargets() {
	static std::vector<std::string> targets = {"Atheism", "Climate Change is a Real Concern", "Feminist Movement", "Hillary Clinton", "Legalization of Abortion", "Donald Trump"};
	return targets;
}

const std::array<std::vector<std::string>, 6> &getTfidfWords() {
    const static std::vector<std::string> ATHEISM_TFIDF = { "atheist", "#atheism", "atheism", "#atheist", "god", "religion", "@atheist_roo", "#god", "#jesus", "@atheist_eh", "#religion", "believe", "dawkins", "#atheists", "clock", "#bible", "@atheist_dh", "november", "am", "richard", "religious", "islam", "about", "atheists", "at", "can", "all", "how", "someone", "really", "from", "we", "@cenkuygur", "like", "#islam", "@rickygervais", "people", "there", "christian", "faith", "@samharrisorg", "because", "boy", "just", "@skepticnikki", "quran", "harris", "compares", "man", "know", "say", "one", "@atheist_krishna", "isis", "when", "out", "@atxbantamathy", "jesus", "think", "should", "@atheist_bot", "only", "@godlessutopia", "@oldrifleman", "did", "#christ", "sam", "up", "true", "#love", "were", "here", "more", "@egy_atheist", "#lord", "new", "evidence", "belief", "muslim", "his", "good", "then", "even", "@richarddawkins", "ahmed", "read", "everyone", "time", "@ryelwilliams", "thanksgiving", "tell", "yes", "muslims", "being", "ricky", "@amadeusalmighty", "gervais", "after", "militant", "values" };
    const static std::vector<std::string> CLIMATE_CHANGE_TFIDF = { "climate", "change", "#nowph", "paris", "support", "po", "campaign", "#cop21", "@r_faulkerson", "action", "#climate", "one", "ambassadors", "summit", "help", "youth", "take", "talks", "at", "science", "@aldenrichards02", "call", "aldub", "#climatechange", "nation", "world", "us", "let", "re", "natin", "we", "pa", "yung", "trend", "naman", "global", "our", "conference", "@allscienceglobe", "awareness", "about", "can", "obama", "rebuke", "all", "development", "isis", "climate-friendly", "recognized", "rice", "best", "from", "alden", "more", "how", "terrorists", "@nowph_org", "new", "bae", "un", "htt¡­", "leaders", "powerful", "chage", "join", "nov", "#actonclimate", "plan", "scientists", "report", "energy", "march", "national", "snow", "says", "deal", "government", "precip", "meet", "study", "great", "blog", "exxon", "day", "now", "record", "greg", "part", "fight", "agreement", "africa", "up", "@ofctrendsetter", "need", "india", "than", "children", "could", "books", "here" };
    const static std::vector<std::string> FEMINIST_MOVEMENT_TFIDF = { "feminist", "feminism", "jumpman", "woman", "jump", "disgusting", "jumpma-", "jumptrans", "men", "wrong", "need", "@feministculture", "too", "women", "#feminism", "bread", "yeast", "about", "so¡­", "vagina", "makes", "blogger", "@blackpplvines", "s¡­", "own", "@lordflaconegro", "from", "documentary", "charli", "boldly", "xcx", "vegan", "white", "just", "gender", "all", "like", "because", "we", "out", "equality", "music", "rape", "being", "intersectional", "feminists", "@woridstarhiph0p", "@shujarabbani", "when", "issue", "industry", "#feminist", "diversity", "mc", "ruth_blane", "most", "against", "how", "think", "officer", "watch", "until", "#heforshe", "sourdough", "people", "everything", "twitter", "love", "muslim", "man", "#16days", "@charli_xcx", "benefits", "sends", "inequality", "than", "fetty", "f-word", "wap", "better", "@byetyler", "caitlyn", "jenner", "gay¡­", "found", "actually", "geller", "threat", "make", "now", "pamela", "@un", "girl", "misuse", "u", "some", "later", "up", "@worldstarc0medy", "at" };
    const static std::vector<std::string> HILLARY_CLINTON_TFIDF = { "hillary", "clinton", "@hillaryclinton", "sanders", "cnn", "emails", "bernie", "reporter", "trump", "illegal", "paul", "rand", "aide", "iowa", "bill", "#hillary", "at", "poll", "over", "lies", "immigrants", "smear", "#benghazi", "@berniesanders", "benghazi", "how", "#hillaryclinton", "from", "campaign", "#hillary2016", "show", "coordinated", "like", "about", "says", "leaked", "#uniteblue", "women", "#hillaryforprison2016", "than", "vote", "more", "attack", "just", "out", "suspended", "video", "fundraiser", "against", "we", "right", "tonight", "worked", "@realdonaldtrump", "speaking", "term", "new", "firm", "#tcot", "did", "hearing", "all", "up", "reveal", "mason", "people", "during", "philadelphia", "gop", "obama", "investment", "news", "fired", "saying", "team", "city", "now", "unethical", "after", "behavior", "ibd", "get", "target", "#pjnet", "state", "lead", "should", "make", "@randpaul", "clinton¡¯s", "want", "#clinton", "access", "#feelthebern", "health", "when", "president", "can", "policy", "politicians" };
    const static std::vector<std::string> ABORTION_TFIDF = { "abortion", "#abortion", "#thanksgivingclapback", "gay", "law", "third", "pass", "potatoes", "mashed", "@kareemdant", "choice", "wisconsin", "being", "please", "anti-abortion", "abortions", "judge", "clinic", "parenthood", "#prolife", "planned", "down", "court", "@lifenewshq", "should", "women", "about", "after", "life", "finesses", "scandal", "stance", "rubio", "having", "criticized", "baby", "we", "now", "when", "activist", "providers", "silent", "unconstitutional", "rights", "strikes", "just", "had", "democrats", "doctors", "laws", "at", "negative", "listen", "plate", "appointed", "ronald", "night", "reagan", "pro-life", "killing", "cruz", "did", "effects", "clinics", "babies", "#prochoice", "like", "against", "dealing", "their", "birth", "@ppact", "ted", "restrictions", "one", "provider", "missouri", "get", "surrogate", "murder", "demands", "@lilagracerose", "triplets", "people", "legal", "want", "#defundpp", "4th", "never", "abc", "out", "pregnant", "ireland", "how", "only", "aunt", "woman", "privileges", "showing", "#best" };
    const static std::vector<std::string> TRUM_TFIDF = { "trump", "donald", "@realdonaldtrump", "hillary", "clinton", "cruz", "#trump", "mcdonald", "laquan", "shooting", "obama", "rubio", "#trump2016", "fox", "debate", "video", "says", "like", "wins", "@hillaryclinton", "president", "his", "vote", "over", "palin", "statement", "from", "gop", "run", "another", "sanders", "out", "calls", "at", "jeb", "we", "#laquanmcdonald", "watch", "kasich", "@anncoulter", "cnn", "other", "him", "than", "how", "win", "can", "just", "laughing", "country", "trump¡¯s", "against", "get", "about", "said", "anyone", "america", "news", "poll", "one", "beat", "think", "immigration", "all", "muslim", "up", "hate", "justified", "@sarahkendzior", "republican", "hospitals", "@danscavino", "expert", "now", "most", "man", "c¡­", "posts", "bernie", "christie", "bombing", "when", "people", "tacos", "corruption", "more", "week", "ad", "sarah", "benghazi", "sc", "event", "only", "us", "vs", "new", "polls", "great", "want", "nevada" };
    const static std::array<std::vector<std::string>, 6> TFIDF_ARR = {ATHEISM_TFIDF, CLIMATE_CHANGE_TFIDF, FEMINIST_MOVEMENT_TFIDF, HILLARY_CLINTON_TFIDF, ABORTION_TFIDF, TRUM_TFIDF};
    return TFIDF_ARR;
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
