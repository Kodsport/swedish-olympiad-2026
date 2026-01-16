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


print(n, b)

# good value first
print(1)
print(1, 5)

for i in range(n-2):
    print(1)
    print(1, 1)

# strength = n - 2 + 5 = n + 3

print(10)
for x in range(9):
    print(n + 8 + x, 1)

print(10, 100)