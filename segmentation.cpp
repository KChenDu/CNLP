#include <string>
#include <unordered_set>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

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

class Node
{
public:
    unordered_map<wchar_t, Node*> children;
    string value;

    Node() : value("")
    {}

    Node(const string& value) : value(value)
    {}

    Node* const add_child(const wchar_t c, const string& value, const bool overwrite=false)
    {
        if (children.find(c) == children.cend())
        {
            Node* const child = new Node(value);
            children[c] = child;
            return child;
        }
        if (overwrite)
        {
            Node* const child = children[c];
            child->value = value;
            return child;
        }
        return children[c];
    }
};

class Trie: public Node
{
public:
    const bool contains(const wstring& key)
    {
        Node* state = this;
        for (const wchar_t c : key)
        {
            unordered_map<wchar_t, Node*>& children = state->children;
            if (children.find(c) == children.cend())
                return false;
            state = state->children[c];
        }
        return state->value.length() > 0;
    }

    string& getitem(const wstring& key)
    {
        Node* state = this;
        for (const wchar_t c : key)
            state = state->children[c];
        return state->value;
    }

    const void setitem(const wstring& key, const string& value)
    {
        Node* state = this;
        const int length = key.length();
        for (int i = 0; i < length - 1; ++i)
            state = state->add_child(key[i], "", false);
        state->add_child(key.back(), value, true);
    }
};

PYBIND11_MODULE(segmentation, m)
{
    m.doc() = "分词";
    m.def("fully_segment", &fully_segment, "完全切分");
    m.def("forward_segment", &forward_segment, "正向最长匹配");
    m.def("backward_segment", &backward_segment, "逆向最长匹配");
    m.def("bidirectional_segment", &bidirectional_segment, "双向最长匹配");
    m.def("evaluate_speed", &evaluate_speed, "速度评测");
    py::class_<Trie>(m, "Trie")
            .def(py::init<>())
            .def("__contains__", &Trie::contains)
            .def("__getitem__", &Trie::getitem)
            .def("__setitem__", &Trie::setitem);
}
