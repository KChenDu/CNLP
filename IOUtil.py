def load_dictionary() -> set:
    dic = set()
    with open("data/dictionary/CoreNatureDictionary.mini.txt", 'r', encoding="utf-8") as file:
        for line in file:
            dic.add(line[:line.find('\t')])
    return dic
