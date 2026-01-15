#!/usr/bin/python3

import sys
import random
import math
from random import randint

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
m = int(cmdlinearg('m'))
q = int(cmdlinearg('q'))
maxw = int(cmdlinearg('maxw'))

countries = []
amount_left = m

assert n <= m

amount_left -= n
for i in range(n):
    countries.append([])
    r = random.randint(0, amount_left)
    for j in range(r + 1):
        w = random.randint(1, maxw)
        countries[i].append(w)
    amount_left -= r

random.shuffle(countries)

queries = []

for i in range(q):
    queries.append(random.randint(1, maxw))

print(n, q)

for i in range(n):
    print(len(countries[i]), *countries[i])

for query in queries:
    print(query)
