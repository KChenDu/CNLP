from IOUtil import load_dictionary
from segmentation import Trie


if __name__ == '__main__':
    trie: Trie = Trie()
    trie['自然'] = 'nature'
    trie['自然人'] = 'human'
    trie['自然语言'] = 'language'
    trie['自语'] = 'talk to oneself'
    trie['入门'] = 'introduction'
    assert '自然' in trie

    trie.remove_item('自然')
    assert '自然' not in trie

    trie['自然语言'] = 'human language'
    assert trie['自然语言'] == 'human language'

    assert trie['入门'] == 'introduction'

    dic: set = load_dictionary()

    m: dict = {}
    for word in dic:
        m[word] = ''

    trie: Trie = Trie(m)

    print(trie.parse_text('商品和服务'))
    print(trie.parse_text('就读北京大学'))

    print(trie.parse_longest_text('就读北京大学'))
    print(trie.parse_longest_text('研究生命起源'))
