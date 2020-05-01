#!/usr/bin/env python3
import csv, yaml, itertools

class Counter(dict) :
    def __missing__(self, key):
        return 0

charset = set()
digram = Counter()
trigram = Counter()
insert = Counter()
enter = Counter()
delete = Counter()

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
ctxs = [i+j for i in charset.replace('$', '') for j in charset.replace('^', '')]

for line in open('ops.txt').readlines() :
    op, ctx, tag, freq = line.split('\t', 4)
    if ctx not in ctxs : continue
    freq = int(freq)
    if op == 'Insert' :
        insert[ctx+tag] += freq
    elif op == 'Delete' :
        delete[ctx] += freq
    elif op == 'Enter' :
        enter[ctx+tag] += freq

stat = {
    'charset': charset,
    # 'laplace': laplace,
    # 'digram': dict(digram), 
    # 'trigram': dict(trigram),
    'edit' : {
        'enter': dict(enter),
        'insert': dict(insert),
        'delete': dict(delete),
        # 'swap': dict(swap)
    }
}

yaml.safe_dump(stat, open('model.yml', 'w'))