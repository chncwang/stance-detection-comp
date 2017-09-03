#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <bitset>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/find.hpp>

using std::vector;
using std::string;
using std::cout;
using std::array;
using std::endl;
using std::bitset;
using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::sort;
using std::set;

unordered_map<string, int> df_map;
int N;

bool ContainChar(const string word) {
  for (char c : word) {
    if (c >= 'a' && c<= 'z') {
      return true;
    }
  }
  return false;
}

void BuildDf(const vector<string> &lines) {
  N = lines.size();
    for (const string &line : lines) {
      vector<string> words;
      boost::split(words, line, boost::is_any_of(" "));
      set<string> word_set;
      for (string &w : words) {
        string lowercase = w;
        boost::algorithm::to_lower(lowercase);
        word_set.insert(lowercase);
      }

      for (const string &w : word_set) {
        auto it = df_map.find(w);
        if (it == df_map.end()) {
          df_map[w] = 1;
        } else {
          df_map[w]++;
        }
      }
    }
}

float GetIdf(const string &word) {
  auto it = df_map.find(word);
  if (it == df_map.end()) {
    abort();
  } else {
    return log(static_cast<float>(N)/it->second);
  }
}

int main() {
  std::vector<std::string> lines;
  std::ifstream input(string("/home/wqs/tweets.txt"));
  int debugi = 0;
  for (string line; std::getline(input, line);) {
    lines.push_back(line);
  }
  BuildDf(lines);
  cout << "lines size:" << lines.size() << endl;

  vector<string> trump_keys = {"trump", "donald"};
  vector<string> hillary_keys = {"hillary", "clinton"};
  vector<string> climate_keys = {"climate"};
  vector<string> atheism_keys = {"theism", "theist"};
  vector<string> feminist_keys = {"feminist", "feminism"};
  vector<string> abortion_keys = {"abortion", "prochoice"};
  unordered_set<string> stop_words = {"a", "to", "the", "is", "and",
    "you", "rt", "'s", "on", "in", "your", "an", "for", "was", "'re",
  "that", "of", "it", "be", "&gt;", "&amp;", "n't", "with", "as", "do", "not",
  "me", "what", "or", "so", "he", "will", "if", "her", "she", "via", "i", "this",
  "but","have", "are", "why", "by", "who", "has","'m", "would", "aside", "my",
  "no", "does", "they"};
//  unordered_set<string> stop_words = {};

  array<vector<string>, 6> targets_keywords = {trump_keys, hillary_keys, climate_keys, atheism_keys, feminist_keys, abortion_keys};
  vector<bitset<6>> tweet_class_vector;
  array<vector<string>, 6> classifiedTweets;

  int x = 0;
  for (string &line : lines) {
    bitset<6> tweet_class;
    int class_count = 0;
    for (int i = 0; i< 6; ++i) {
      auto &v = targets_keywords.at(i);
      for (string & keyword : v) {
        if (boost::ifind_first(line, keyword)) {
          classifiedTweets.at(i).push_back(line);
          tweet_class.set(i);
//          if (++class_count > 1) {
//            cout << "multi class :" + line << endl;
//          }
          break;
        }
      }
    }
    int count = tweet_class.count();

    tweet_class_vector.push_back(tweet_class);
  }

  for (int i = 0; i< 6; ++i) {
    cout << classifiedTweets.at(i).size() << endl;

    unordered_map<string, int> word_count_map;
    for (const string &line : classifiedTweets.at(i)) {
      vector<string> segs;
      boost::split(segs, line, boost::is_any_of(" "));
      for (const string& seg : segs) {
        string lowercase = seg;
        boost::algorithm::to_lower(lowercase);

        if (!ContainChar(lowercase) ||
            stop_words.find(lowercase) != stop_words.end()) {
          continue;
        }
        if (boost::starts_with(lowercase, "http")) {
          continue;
        }
        if (word_count_map.find(lowercase) == word_count_map.end()) {
          word_count_map[lowercase] = 1;
        } else {
          word_count_map[lowercase]++;
        }
      }
    }
    vector<pair<string, int>> pairs;
    for (auto it : word_count_map) {
      pairs.push_back(pair<string, int>(it.first, it.second));
    }

    cout << targets_keywords.at(i).at(0) << endl;
    sort(pairs.begin(), pairs.end(),
        [=](const pair<string, int> &a, const pair<string, int> &b) {
        return a.second * GetIdf(a.first) > b.second * GetIdf(b.first);
        });
    for (int i = 0; i< 100; ++i) {
      cout <<"\"" << pairs.at(i).first << "\", ";
    }
    cout << std::endl;
  }

  return 0;
}
