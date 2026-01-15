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

countries = [[] for _ in range(n+1)]



s = int(cmdlinearg('s', n//2))
islands = set()
while len(islands) < s:
    islands.add(random.randint(1,n))

noshow = [x for x in range(1,n+1) if x not in islands]
noshow.sort()
islands = list(islands)
islands.sort()

weights = []
while len(weights) < m:
    weights.append(random.randint(1,maxw))

weights.sort()

queries = random.sample(weights,q)

for i in islands:
    countries[i].append(weights.pop())

for i in noshow:
    countries[i].append(weights.pop())
    countries[i].append(weights.pop())

while weights:
    countries[random.randint(1,n)].append(weights.pop())

random.shuffle(queries)

noise = int(cmdlinearg('noise',0))

print(n,q)

for i in range(1,n+1):
    random.shuffle(countries[i])
    print(len(countries[i]),*countries[i])

for x in queries:
    print(max(1,min(x + random.randint(-1,1)*noise, maxw)))
