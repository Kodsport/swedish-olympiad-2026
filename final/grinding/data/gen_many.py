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
num_weird = int(cmdlinearg('num_weird'))

weird_at = set(random.sample(range(1,n), num_weird))

print(n, b)
for i in range(n):
    print(1)
    if i in weird_at:
        print(randint(1, 10), randint(1, 10))
    else:
        print(1, 1)
