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
n = int(cmdlinearg('n', random.randint(1,10)))
a = int(cmdlinearg('a', random.randint(0,10)))
b = int(cmdlinearg('b', random.randint(0,10)))

s = cmdlinearg('s')

print(n,a,b)
print("".join(s))
