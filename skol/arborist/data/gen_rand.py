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
k = int(cmdlinearg('k'))
print(n, k)

same_coord = int(cmdlinearg('same_coord', 0))
if same_coord:
    coord = randint(1, 30)
    coords = [coord] * n
else:
    coords = (randint(1, 30) for _ in range(n))
print(*coords)


weights = [20, 40]
weight_20 = int(cmdlinearg('weight_20', 0))
if weight_20:
    weights = [20]
weights = [w for w in weights if w <= k]
print(*(random.choice(weights) for _ in range(n)))
