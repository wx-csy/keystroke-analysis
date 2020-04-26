#!/usr/bin/env python3
import csv, yaml, itertools

class Counter(dict) :
    def __missing__(self, key):
        return 0

charset = set()
digram = Counter()
trigram = Counter()
insert = Counter()
delete = Counter()
replace = Counter()
swap = Counter()

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

for op, freq in csv.reader(open('ops.txt'), delimiter='\t') :
    freq = int(freq)
    op, old = op.split(' ', 1)
    old, new = old.split(' -> ', 1)
    if op == 'Insert' :
        insert[new] += 1
    elif op == 'Delete' :
        delete[old] += 1
    elif op == 'Swap' :
        swap[new] += 1
    elif op == 'Replace' :
        replace[new[0] + new[1] + old[1] + new[2]] += 1

laplace = tot_freq // (2 * tot_sentence) + 1

stat = {
    'charset': charset,
    'laplace': laplace,
    'digram': dict(digram), 
    'trigram': dict(trigram),
    'edit' : {
        'replace': dict(replace),
        'insert': dict(insert),
        'delete': dict(delete),
        'swap': dict(swap)
    }
}

yaml.safe_dump(stat, open('result.yml', 'w'))