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
l = int(cmdlinearg('l'))

mode = cmdlinearg("mode")


print(n,t)
items = []
for i in range(n):
    if mode == "random":
        items.append((randint(1, l), randint(1, t)))
    elif mode=="corr":
        profit = randint(1, l)
        mul = int(cmdlinearg('mul'))
        add = int(cmdlinearg('add'))
        items.append((profit, profit*randint(mul-2, mul+2) + randint(1, add)))
    elif mode=="stair":
        profit = randint(1, l)
        len = int(cmdlinearg('len'))
        scale = int(cmdlinearg('scale'))
        items.append((profit, 1+i//len*scale))
    else:
        assert 0
random.shuffle(items)
[print(*item) for item in items]
