from time import time


def fully_segment(text: str, dic: set[str]) -> list[str]:
    word_list: list[str] = []
    length: int = len(text)
    for i in range(length):
        for j in range(i + 1, length + 1):
            word: str = text[i:j]
            if word in dic:
                word_list.append(word)
    return word_list


def count_single_char(word_list: list[str]) -> int:
    sum: int = 0
    for word in word_list:
        if len(word) < 2:
            sum += 1
    return sum


def forward_segment(text: str, dic: set[str]) -> list[str]:
    word_list: list[str] = []
    length: int = len(text)
    i: int = 0
    while i < length:
        longest_word: str = text[i]
        for j in range(length, i + 1, -1):
            word: str = text[i:j]
            if word in dic:
                longest_word = word
                break
        word_list.append(longest_word)
        i += len(longest_word)
    return word_list


def backward_segment(text: str, dic: set[str]) -> list[str]:
    word_list: list[str] = []
    i: int = len(text) - 1
    while i > -1:
        longest_word: str = text[i]
        for j in range(0, i):
            word: str = text[j:i + 1]
            if word in dic:
                longest_word = word
                break
        word_list.append(longest_word)
        i -= len(longest_word)
    word_list.reverse()
    return word_list


def bidirectional_segment(text: str, dic: set[str]) -> list[str]:
    f: list[str] = forward_segment(text, dic)
    b: list[str] = backward_segment(text, dic)
    length_f: int = len(f)
    length_b: int = len(b)
    if length_f < length_b:
        return f
    if length_f > length_b:
        return b
    if count_single_char(f) < count_single_char(b):
        return f
    return b


def evaluate_speed(segment, text: str, dic: set[str], pressure: int = 10000) -> float:
    start_time: float = time()
    for _ in range(pressure):
        segment(text, dic)
    print(f'{len(text) * pressure / 10000. / (time() - start_time):.2f} 万字/秒')
