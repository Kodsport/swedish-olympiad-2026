#!/usr/bin/python3

import sys
import random
import math

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
r = int(cmdlinearg('r'))
c = int(cmdlinearg('c'))
p_blocked = float(cmdlinearg('p_blocked', 0.3))

print(r,c)
grid = []
for i in range(r):
    row = "".join('#' if random.random() < p_blocked else '.' for j in range(c))
    grid.append(row)
    print(row)

def is_blocked(a,b):
    return grid[a%r][b%c] == '#'

max_coord = int(cmdlinearg('max_coord'))
for _ in range(5):
    while True:
        r1 = random.randint(0, max_coord)
        c1 = random.randint(0, max_coord)
        r2 = random.randint(0, max_coord)
        c2 = random.randint(0, max_coord)
        if is_blocked(r1,c1) or is_blocked(r2,c2):
            continue
        print(r1, c1, r2, c2)
        break
