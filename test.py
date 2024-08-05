from IOUtil import load_dictionary
from segmentation import Trie


if __name__ == '__main__':
    trie = Trie()
    trie['自然'] = 'nature'
    trie['自然人'] = 'human'
    trie['自然语言'] = 'language'
    trie['自语'] = 'talk to oneself'
    trie['入门'] = 'introduction'
    assert '自然' in trie

    trie['自然'] = ''
    assert '自然' not in trie

    trie['自然语言'] = 'human language'
    assert trie['自然语言'] == 'human language'

    assert trie['入门'] == 'introduction'

    dic = load_dictionary()

    m = {}
    for word in dic:
        m[word] = '\0'

    trie = Trie(m)

    print(trie.parse_text('商品和服务'))
    print(trie.parse_text('就读北京大学'))

    print(trie.parse_longest_text('就读北京大学'))
    print(trie.parse_longest_text('研究生命起源'))
