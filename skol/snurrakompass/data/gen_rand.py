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

ab = int(cmdlinearg('ab', 0))
prime = int(cmdlinearg('prime', 0))

if prime == 1:
    a = 13
    b = randint(1, 360)
    print(a, b)
elif ab == 1:
    a = randint(1, 360) 
    print(a, a)
else:
    a = randint(1, 360)
    b = randint(1, 360)
    print(a, b)


