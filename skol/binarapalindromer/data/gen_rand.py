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
a = int(cmdlinearg('a',random.randint(1,10**18)))
b = int(cmdlinearg('b',random.randint(a,10**18)))
k = int(cmdlinearg('k',random.randint(1,round(len(bin(b)[2:])))))

print(a,b,k)

