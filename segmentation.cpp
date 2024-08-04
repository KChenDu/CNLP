#include <string>
#include <unordered_set>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

namespace py = pybind11;
using namespace std;

const vector<wstring> fully_segment(const wstring& text, const unordered_set<wstring>& dic)
{
    vector<wstring> word_list;
    const int length = text.length();
    for (int i = 0; i < length; ++i)
        for (int n = 1; i + n <= length; ++n)
        {
            const wstring word = text.substr(i, n);
            if (dic.find(word) != dic.cend())
               word_list.push_back(word);
        }
    return word_list;
}

const uint count_single_char(const vector<wstring>& word_list)
{
    uint sum = 0u;
    for (const wstring& word : word_list)
        if (word.length() < 2)
            ++sum;
    return sum;
}

const vector<wstring> forward_segment(const wstring& text, const unordered_set<wstring>& dic)
{
    vector<wstring> word_list;
    const int length = text.length();
    int i = 0;
    while (i < length)
    {
        wstring longest_word = text.substr(i, 1);
        for (int n = length - i; n > 1; --n)
        {
            const wstring word = text.substr(i, n);
            if (dic.find(word) != dic.cend())
            {
                longest_word = word;
                break;
            }
        }
        word_list.push_back(longest_word);
        i += longest_word.length();
    }
    return word_list;
}

const vector<wstring> backward_segment(const wstring& text, const unordered_set<wstring>& dic)
{
    vector<wstring> word_list;
    int i = text.length() - 1;
    while (i > -1)
    {
        wstring longest_word = text.substr(i, 1);
        for (int n = i + 1; n > 1; --n)
        {
            const wstring word = text.substr(i + 1 - n, n);
            if (dic.find(word) != dic.cend())
            {
                longest_word = word;
                break;
            }
        }
        word_list.push_back(longest_word);
        i -= longest_word.length();
    }
    reverse(word_list.begin(), word_list.end());
    return word_list;
}

const vector<wstring> bidirectional_segment(const wstring& text, const unordered_set<wstring>& dic)
{
    const vector<wstring> f = forward_segment(text, dic), b = backward_segment(text, dic);
    const int f_size = f.size(), b_size = b.size();
    if (f_size < b_size)
        return f;
    if (f_size > b_size)
        return b;
    if (count_single_char(f) < count_single_char(b))
        return f;
    return b;
}

void evaluate_speed(const function<const vector<wstring>(const wstring&, const unordered_set<wstring>&)>& segment, const wstring& text, const unordered_set<wstring>& dic, const int pressure=10000)
{
    const clock_t start_time = clock();
    for (int i = 0; i < pressure; ++i)
        segment(text, dic);
    cout << fixed << setprecision(2) << text.length() * pressure / 10000. / (clock() - start_time) * CLOCKS_PER_SEC << " 万字/秒" << endl;
}

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
Node<Type>::Node() : value(Type())
{}

template <class Type>
Node<Type>::Node(const Type& value) : value(value)
{}

template <class Type>
Node<Type>* const Node<Type>::add_child(const wchar_t c, const Type& value, const bool overwrite)
{
    if (children.find(c) == children.cend())
    {
        Node<Type>* const child = new Node(value);
        children[c] = child;
        return child;
    }
    if (overwrite)
    {
        Node<Type>* const child = children[c];
        child->value = value;
        return child;
    }
    return children[c];
}

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

template <class Type>
Trie<Type>::Trie()
{}

template <class Type>
const void Trie<Type>::setitem(const wstring& key, const Type& value)
{
    Node<Type>* state = this;
    const int length = key.length();
    for (int i = 0; i < length - 1; ++i)
        state = state->add_child(key[i], Type(), false);
    state->add_child(key.back(), value, true);
}

template <class Type>
Trie<Type>::Trie(const unordered_map<wstring, Type>& dic)
{
    for (const auto& [word, value] : dic)
        setitem(word, value);
}

template <class Type>
const bool Trie<Type>::contains(const wstring& key)
{
    Node<Type>* state = this;
    for (const wchar_t c : key)
    {
        unordered_map<wchar_t, Node<Type>*>& children = state->children;
        if (children.find(c) == children.cend())
            return false;
        state = children[c];
    }
    return state->value != Type();
}

template <class Type>
Type& Trie<Type>::operator[](const wstring& key)
{
    Node<Type>* state = this;
    for (const wchar_t c : key)
        state = state->children[c];
    return state->value;
}

template <class Type>
const vector<wstring> Trie<Type>::parse_text(const wstring& text)
{
    vector<wstring> word_list;
    const int length = text.length();
    for (int i = 0; i < length; ++i)
    {
        Node<Type>* state = this;
        for (int j = i; j < length; ++j)
        {
            unordered_map<wchar_t, Node<Type>*>& children = state->children;
            if (children.find(text[j]) == children.cend())
                break;
            state = children[text[j]];
            if (state->value != Type())
                word_list.push_back(text.substr(i, j - i + 1));
        }
    }
    return word_list;
}

template <class Type>
const vector<wstring> Trie<Type>::parse_longest_text(const wstring& text)
{
    vector<wstring> word_list;
    const int length = text.length();
    int i = 0;
    while (i < length)
    {
        Node<Type>* state = this;
        int n = 1;
        for (int j = i; j < length; ++j)
        {
            unordered_map<wchar_t, Node<Type>*>& children = state->children;
            if (children.find(text[j]) == children.cend())
                break;
            state = children[text[j]];
            if (state->value != Type())
                n = j - i + 1;
        }
        word_list.push_back(text.substr(i, n));
        i += n;
    }
    return word_list;
}

const vector<wstring> fully_segment(const wstring& text, Trie<string>& dic)
{
    return dic.parse_text(text);
}

const vector<wstring> forward_segment(const wstring& text, Trie<string>& dic)
{
    return dic.parse_longest_text(text);
}

const vector<wstring> backward_segment(const wstring& text, Trie<string>& dic)
{
    vector<wstring> word_list;
    int i = text.length() - 1;
    while (i > -1)
    {
        wstring longest_word = text.substr(i, 1);
        for (int n = i + 1; n > 1; --n)
        {
            const wstring word = text.substr(i + 1 - n, n);
            if (dic.contains(word))
            {
                longest_word = word;
                break;
            }
        }
        word_list.push_back(longest_word);
        i -= longest_word.length();
    }
    reverse(word_list.begin(), word_list.end());
    return word_list;
}

const vector<wstring> bidirectional_segment(const wstring& text, Trie<string>& dic)
{
    const vector<wstring> f = forward_segment(text, dic), b = backward_segment(text, dic);
    const int f_size = f.size(), b_size = b.size();
    if (f_size < b_size)
        return f;
    if (f_size > b_size)
        return b;
    if (count_single_char(f) < count_single_char(b))
        return f;
    return b;
}

PYBIND11_MODULE(segmentation, m)
{
    m.doc() = "分词";
    m.def("fully_segment", py::overload_cast<const wstring&, const unordered_set<wstring>&>(&fully_segment), "完全切分");
    m.def("forward_segment", py::overload_cast<const wstring&, const unordered_set<wstring>&>(&forward_segment), "正向最长匹配");
    m.def("backward_segment", py::overload_cast<const wstring&, const unordered_set<wstring>&>(&backward_segment), "逆向最长匹配");
    m.def("bidirectional_segment", py::overload_cast<const wstring&, const unordered_set<wstring>&>(&bidirectional_segment), "双向最长匹配");
    m.def("evaluate_speed", &evaluate_speed, "速度评测");
    py::class_<Trie<string>>(m, "Trie")
            .def(py::init<>())
            .def(py::init<const unordered_map<wstring, string>&>())
            .def("__contains__", &Trie<string>::contains)
            .def("__getitem__", &Trie<string>::operator[])
            .def("__setitem__", &Trie<string>::setitem)
            .def("parse_text", &Trie<string>::parse_text)
            .def("parse_longest_text", &Trie<string>::parse_longest_text);
    m.def("fully_segment", py::overload_cast<const wstring&, Trie<string>&>(&fully_segment), "完全切分");
    m.def("forward_segment", py::overload_cast<const wstring&, Trie<string>&>(&forward_segment), "正向最长匹配");
    m.def("backward_segment", py::overload_cast<const wstring&, Trie<string>&>(&backward_segment), "逆向最长匹配");
    m.def("bidirectional_segment", py::overload_cast<const wstring&, Trie<string>&>(&bidirectional_segment), "双向最长匹配");
}
