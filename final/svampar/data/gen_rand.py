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
mode = cmdlinearg('mode')

print(n)
if mode == "random":
    shrooms = [randint(0, 255) for _ in range(n)]
elif mode == "big":
    shrooms = [255 for _ in range(n)]
    num_rand = int(cmdlinearg('num_rand'))
    for _ in range(num_rand):
        shrooms[randint(0, n-1)] = randint(0, 255)
elif mode=="num_big":
    shrooms = [randint(0, 127) for _ in range(n)]
    num_big = int(cmdlinearg('num_big'))
    for _ in random.sample(range(n), num_big):
        shrooms[_] = randint(128, 255)
else:
    assert 0
print(*shrooms)

