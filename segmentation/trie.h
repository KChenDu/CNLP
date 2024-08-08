#include <string>
#include <unordered_map>
#include <vector>
#include <pybind11/pybind11.h>

using namespace std;
namespace py = pybind11;

#ifndef CNLP_TRIE_H
#define CNLP_TRIE_H

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

const void init_trie(py::module_ &m);

#endif //CNLP_TRIE_H
