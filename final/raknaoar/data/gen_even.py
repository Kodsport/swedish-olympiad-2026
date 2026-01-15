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

amount = math.floor(m/n)

for i in range(n):
    temp_lis = []
    for j in range(amount):
        temp_lis.append(random.randint(1, maxw))
    countries.append(temp_lis)

rest = m-amount*n
for i in range(rest):
    countries[-1].append(random.randint(1, maxw))

queries = []

for i in range(q):
    queries.append(random.randint(1, maxw))

print(n, q)

for i in range(n):
    print(len(countries[i]), *countries[i])

for query in queries:
    print(query)
