from IOUtil import load_dictionary
from segmentation import pressure, fully_segment, forward_segment, backward_segment, bidirectional_segment, evaluate_speed, Trie
from time import time


if __name__ == '__main__':
    text: str = "江西鄱阳湖干枯，中国最大淡水湖变成大草原"
    dic: set = load_dictionary()

    evaluate_speed(fully_segment, text, dic)
    evaluate_speed(forward_segment, text, dic)
    evaluate_speed(backward_segment, text, dic)
    evaluate_speed(bidirectional_segment, text, dic)
    print('\n')


    def evaluate_speed(segment, text: str, dic: set[str]) -> float:
        start_time: float = time()
        for _ in range(pressure):
            segment(text, dic)
        print(f'{len(text) * pressure / 10000. / (time() - start_time):.2f} 万字/秒')


    m = {}
    for word in dic:
        m[word] = '\0'

    trie = Trie(m)
    evaluate_speed(fully_segment, text, trie)
    evaluate_speed(forward_segment, text, trie)
    evaluate_speed(backward_segment, text, trie)
    evaluate_speed(bidirectional_segment, text, trie)
