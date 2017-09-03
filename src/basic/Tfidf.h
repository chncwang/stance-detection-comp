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
#include "Instance.h"

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
        if (c >= 'a' && c <= 'z') {
            return true;
        }
    }
    return false;
}

void BuildDf(const vector<string> &lines) {
    N = lines.size();
    for (const string &line : lines) {
        vector<string> words;
        split_bychar(line, words);
        set<string> word_set;
        for (string &w : words) {
            string lowercase = w;
            lowercase = normalize_to_lowerwithdigit(lowercase);
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
        return log(static_cast<float>(N) / it->second);
    }
}

const std::array<std::vector<std::string>, 6>& getTfidfWords(const std::string &idf_file, const std::vector<Instance> &instances) {
    static std::array<std::vector<std::string>, 6> *result;
    if (result == NULL) {
        result = new std::array<std::vector<std::string>, 6>();
        std::vector<std::string> lines;
        std::ifstream input(idf_file);
        int debugi = 0;
        for (string line; std::getline(input, line);) {
            lines.push_back(line);
        }
        BuildDf(lines);
        cout << "lines size:" << lines.size() << endl;

        vector<string> trump_keys = { "trump", "donald" };
        vector<string> hillary_keys = { "hillary", "clinton" };
        vector<string> climate_keys = { "climate" };
        vector<string> atheism_keys = { "theism", "theist" };
        vector<string> feminist_keys = { "feminist", "feminism" };
        vector<string> abortion_keys = { "abortion", "prochoice" };
        unordered_set<string> stop_words = { "a", "to", "the", "is", "and",
          "you", "rt", "'s", "on", "in", "your", "an", "for", "was", "'re",
        "that", "of", "it", "be", "&gt;", "&amp;", "n't", "with", "as", "do", "not",
        "me", "what", "or", "so", "he", "will", "if", "her", "she", "via", "i", "this",
        "but","have", "are", "why", "by", "who", "has","'m", "would", "aside", "my",
        "no", "does", "they" };
        //  unordered_set<string> stop_words = {};

        for (int i = 0; i < 6; ++i) {
            unordered_map<string, int> word_count_map;
            for (const auto &ins : instances) {
                if (ins.m_target != i) {
                    continue;
                }
                for (const string& seg : ins.m_tweet_words) {
                    if (!ContainChar(seg) ||
                        stop_words.find(seg) != stop_words.end()) {
                        continue;
                    }
                    if (word_count_map.find(seg) == word_count_map.end()) {
                        word_count_map[seg] = 1;
                    } else {
                        word_count_map[seg]++;
                    }
                }
            }
            vector<pair<string, int>> pairs;
            for (auto it : word_count_map) {
                pairs.push_back(pair<string, int>(it.first, it.second));
            }

            sort(pairs.begin(), pairs.end(),
                [=](const pair<string, int> &a, const pair<string, int> &b) {
                return a.second * GetIdf(a.first) > b.second * GetIdf(b.first);
            });
            for (int j = 0; j < 100; ++j) {
                result->at(i).push_back(pairs.at(j).first);
            }
        }
    }

    return *result;
}
