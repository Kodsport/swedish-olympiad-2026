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
t = int(cmdlinearg('t'))

mode = cmdlinearg("mode")

print(n,t)
items = []
for i in range(n):
    if mode == "nope":
        items.append((1, t+randint(1, 100)))
    elif mode=="one":
        items.append((1, 1))
    elif mode=="arithm":
        items.append((1, i+1))
    else:
        assert 0
random.shuffle(items)
[print(*item) for item in items]
