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
t = int(cmdlinearg('t'))
l = int(cmdlinearg('l'))

assert l>=3

# Make the capacity odd to prevent easy exact partition matches
if t % 2 == 0:
    t -= 1

items = []
# 3 classes with very close ratios and coprime weights
n1 = int(n * 0.35)
n2 = int(n * 0.35)
n3 = n - n1 - n2

for _ in range(n1):
    items.append((l, 4 * l + 1))
for _ in range(n2):
    items.append((l - 1, 4 * (l - 1) + 1))
for _ in range(n3):
    items.append((l - 2, 4 * (l - 2) + 1))

random.shuffle(items)

print(f"{n} {t}")
for p, w in items:
    print(f"{p} {w}")
