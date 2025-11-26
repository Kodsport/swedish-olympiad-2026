#!/usr/bin/python3

import sys
import random
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

from heapq import *

n = int(cmdlinearg('n'))
p = int(cmdlinearg('p'))
h = int(cmdlinearg('h'))
f = int(cmdlinearg('f'))

print(n,p,h,f)
unsolved = [(randint(1, 10**9), team, prob) for team in range(1,n+1) for prob in range(1, p+1)]
heapify(unsolved)
for i in range(h):
    pair = heappop(unsolved)
    _, team, prob = pair
    solves = random.choice(('A','W'))
    print(team, prob, solves)
    if solves=='W':
        heappush(unsolved, (randint(1, 10**9), team, prob))

for i in range(f):
    pair = heappop(unsolved)
    _, team, prob = pair
    solves = random.choice(('A','W'))
    print(team, prob, solves)
    if solves=='W':
        heappush(unsolved, (randint(1, 10**9), team, prob))
