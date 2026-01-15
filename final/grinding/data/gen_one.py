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
t = int(cmdlinearg('t'))
b = int(cmdlinearg('b'))
mul = int(cmdlinearg('mul', 1))
add = int(cmdlinearg('mul', 0))

print(1, b)

monsters = []
for j in range(1,t+1):
    health = j*(j+1)//2 * randint(1, mul) + randint(0, add)
    if j == 1:
        health = 1
    if health > 10**9:
        break
    monsters.append((health, 1))
print(len(monsters))
for m in monsters:
    print(*m)
