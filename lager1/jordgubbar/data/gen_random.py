#!/usr/bin/python3

import sys
import random

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

MAXN = 300
random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n', MAXN))
m = int(cmdlinearg('m', 2))
T = int(cmdlinearg('T'))
mode = cmdlinearg('mode', 'random')

weights = []

if mode == "random":
    weights = [random.randint(1,m) for _ in range(n)]
    weights.sort()

elif mode == "m2":
    ones = int(cmdlinearg("ones", 0))
    weights = [1]*ones + [2]*(n-ones)
    m = 2

elif mode == "list":
    weights = eval(cmdlinearg('w', '[]'))
    weights.sort()

n = len(weights)

print(n,m,T)
print(*weights)

