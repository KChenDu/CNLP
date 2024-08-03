from IOUtil import load_dictionary
from pysegmentation import forward_segment, backward_segment, bidirectional_segment, evaluate_speed


if __name__ == '__main__':
    text: str = "江西鄱阳湖干枯，中国最大淡水湖变成大草原"
    pressure: int = 10000
    dic: set = load_dictionary()


    from pysegmentation import forward_segment, backward_segment, bidirectional_segment, evaluate_speed


    evaluate_speed(forward_segment, text, dic, pressure)
    evaluate_speed(backward_segment, text, dic, pressure)
    evaluate_speed(bidirectional_segment, text, dic, pressure)
    print('\n')


    from segmentation import forward_segment, backward_segment, bidirectional_segment, evaluate_speed


    evaluate_speed(forward_segment, text, dic, pressure)
    evaluate_speed(backward_segment, text, dic, pressure)
    evaluate_speed(bidirectional_segment, text, dic, pressure)
    print('\n')


    from segmentation import forward_segment, backward_segment, bidirectional_segment, Trie
    from pysegmentation import evaluate_speed


    trie = Trie()
    for word in dic:
        trie[word] = '\0'

    evaluate_speed(forward_segment, text, trie, pressure)
    evaluate_speed(backward_segment, text, trie, pressure)
    evaluate_speed(bidirectional_segment, text, trie, pressure)
