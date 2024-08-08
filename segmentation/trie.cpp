#include "trie.h"
#include <pybind11/stl.h>

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

const void init_trie(py::module_ &m)
{
    py::class_<Trie<string>>(m, "Trie")
            .def(py::init<>())
            .def(py::init<const unordered_map<wstring, string>&>())
            .def("__contains__", &Trie<string>::contains)
            .def("__getitem__", &Trie<string>::operator[])
            .def("__setitem__", &Trie<string>::setitem)
            .def("parse_text", &Trie<string>::parse_text)
            .def("parse_longest_text", &Trie<string>::parse_longest_text);
}
