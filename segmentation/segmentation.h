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
void evaluate_speed(const function<const vector<wstring>(const wstring&, const unordered_set<wstring>&)>& segment, const wstring& text, const unordered_set<wstring>& dic, const int pressure=10000);

template <class Type>
class Node
{
public:
    unordered_map<wchar_t, Node<Type>*> children;
    Type value;

    Node();
    Node(const Type& value);
    Node<Type>* const add_child(const wchar_t c, const Type& value, const bool overwrite=false);
};

template <class Type>
class Trie: public Node<Type>
{
public:
    Trie();
    const void setitem(const wstring& key, const Type& value);
    Trie(const unordered_map<wstring, Type>& dic);
    const bool contains(const wstring& key);
    Type& operator[](const wstring& key);
    const vector<wstring> parse_text(const wstring& text);
    const vector<wstring> parse_longest_text(const wstring& text);
};

const vector<wstring> fully_segment(const wstring& text, Trie<string>& dic);
const vector<wstring> forward_segment(const wstring& text, Trie<string>& dic);
const vector<wstring> backward_segment(const wstring& text, Trie<string>& dic);
const vector<wstring> bidirectional_segment(const wstring& text, Trie<string>& dic);

#endif //CNLP_SEGMENTATION_H
