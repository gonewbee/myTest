import sys

if __name__ == '__main__':
    """
    统计文本单词长度大于指定长度且出现频率最高的单词
    $ python FrequencyCounter.py 1 tinyTale.txt
    the 10
    $ python FrequencyCounter.py 8 tale.txt
    business 122
    $ python FrequencyCounter.py 10 leipzig1M.txt
    government 24763
    """
    if len(sys.argv) < 3:
        print('Need input file')
        sys.exit(1)
    d = {}
    with open(sys.argv[2], 'r') as f:
        length = int(sys.argv[1])
        for l in f.readlines():
            for word in l.split():
                if len(word) < length:
                    continue
                if word in d:
                    d[word] = d[word] + 1
                else:
                    d[word] = 1
        maxWord = ""
        d[maxWord] = 0
        for word in d.keys():
            if d[word] > d[maxWord]:
                maxWord = word
        print(maxWord, d[maxWord])
