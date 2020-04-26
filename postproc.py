#!/usr/bin/env python3
import csv, yaml, itertools

class Counter(dict) :
    def __missing__(self, key):
        return 0

charset = set()
digram = Counter()
trigram = Counter()

tot_sentence = 0
tot_freq = 0

for sentence, freq in csv.reader(open('sentences.txt'), delimiter='\t') :
    tot_sentence += 1
    sentence = '^' + sentence + '$';
    freq = int(freq)
    tot_freq += freq
    charset |= set(sentence)
    for i in range(len(sentence) - 1) :
        digram[sentence[i:i+2]] += freq
    for i in range(len(sentence) - 2) :
        trigram[sentence[i:i+3]] += freq

charset = ''.join(sorted(charset))

laplace = tot_freq // (2 * tot_sentence) + 1

stat = {
    'charset': charset,
    'laplace': laplace,
    'digram': dict(digram), 
    'trigram': dict(trigram),
}

yaml.safe_dump(stat, open('result.yml', 'w'))