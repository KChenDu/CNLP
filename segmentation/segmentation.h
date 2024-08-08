#include <string>
#include <vector>
#include <unordered_set>
#include <functional>

using namespace std;

#ifndef CNLP_SEGMENTATION_H
#define CNLP_SEGMENTATION_H

const vector<wstring> fully_segment(const wstring& text, const unordered_set<wstring>& dic);
const uint count_single_char(const vector<wstring>& word_list);
const vector<wstring> forward_segment(const wstring& text, const unordered_set<wstring>& dic);
const vector<wstring> backward_segment(const wstring& text, const unordered_set<wstring>& dic);
const vector<wstring> bidirectional_segment(const wstring& text, const unordered_set<wstring>& dic);
const void evaluate_speed(const function<const vector<wstring>(const wstring&, const unordered_set<wstring>&)>& segment, const wstring& text, const unordered_set<wstring>& dic, const int pressure=10000);
const vector<wstring> fully_segment(const wstring& text, Trie<string>& dic);
const vector<wstring> forward_segment(const wstring& text, Trie<string>& dic);
const vector<wstring> backward_segment(const wstring& text, Trie<string>& dic);
const vector<wstring> bidirectional_segment(const wstring& text, Trie<string>& dic);

#endif //CNLP_SEGMENTATION_H
