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

x = int(cmdlinearg('x'))

base = int(cmdlinearg('base', 1))

a = [base] + [base<<1]*(x) + [base] + [base<<1]*(x+1)

shift = int(cmdlinearg('shift',0))

a = a[shift:] + a[:shift]

print(len(a))
print(*a)
