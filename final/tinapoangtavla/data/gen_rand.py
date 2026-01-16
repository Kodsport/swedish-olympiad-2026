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

n = int(cmdlinearg('n'))
p = int(cmdlinearg('p'))
h = int(cmdlinearg('h'))
f = int(cmdlinearg('f'))
p_ac = float(cmdlinearg('p_ac', '0.5'))
mode = cmdlinearg('mode')

from heapq import *

def generate_verdict():
    if random.random() < p_ac:
        return 'AC'
    else:
        return 'WA'

print(n, p, h, f)

if mode == 'one_sub_per_problem':
    available = [(team, prob) for team in range(1, n+1) for prob in range(1, p+1)]
    assert h + f <= len(available)
    random.shuffle(available)
    
    for i in range(h):
        team, prob = available[i]
        solves = generate_verdict()
        print(team, prob, solves)
    
    for i in range(h, h + f):
        team, prob = available[i]
        solves = generate_verdict()
        print(team, prob, solves)
elif mode=='random':
    unsolved = [(randint(1, 10**9), team, prob) for team in range(1,n+1) for prob in range(1, p+1)]
    heapify(unsolved)
    for i in range(h):
        pair = heappop(unsolved)
        _, team, prob = pair
        solves = generate_verdict()
        print(team, prob, solves)
        if solves=='WA':
            heappush(unsolved, (randint(1, 10**9), team, prob))

    for i in range(f):
        pair = heappop(unsolved)
        _, team, prob = pair
        solves = generate_verdict()
        print(team, prob, solves)
        if solves=='WA':
            heappush(unsolved, (randint(1, 10**9), team, prob))
else:
    assert 0
