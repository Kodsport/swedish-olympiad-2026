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

step = maxw//m

for i in range(n):
    countries.append([])
current = 1

i = 1
going_forward = True
for j in range(m):
    countries[i].append(current)
    if going_forward:
        if i == len(countries)-1:
            going_forward = False
        else:
            i = i+1
    else:
        if i == 0:
            going_forward = True
        else:
            i = i-1
    current += step

queries = []

for i in range(q):
    queries.append(random.randint(1, maxw))

print(n, q)

for i in range(n):
    print(len(countries[i]), *countries[i])

for query in queries:
    print(query)
