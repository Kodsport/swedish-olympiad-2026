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
b = int(cmdlinearg('b'))
t = int(cmdlinearg('t', n))

assert t >= n

print(n, b)
for i in range(n-1):
    print(1)
    print(1, 1)

curr_strength = n
t -= n-1
dungeon = []
for j in range(0, t):
    strength = curr_strength + j*(j+1)//2
    if strength > 10**9:
        break
    dungeon.append((strength, 1))

print(len(dungeon))
for m in dungeon:
    print(*m)