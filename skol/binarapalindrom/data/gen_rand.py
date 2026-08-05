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

# Bounds for the randomized defaults, so that generator.sh does not have to
# roll the dice itself (bash's $RANDOM is not seeded by testdata_tools, which
# would make the test data unreproducible).
mina = int(cmdlinearg('mina',1))
maxa = int(cmdlinearg('maxa',10**18))
minb = int(cmdlinearg('minb',1))
maxb = int(cmdlinearg('maxb',10**18))

a = int(cmdlinearg('a',random.randint(mina,maxa)))
b = int(cmdlinearg('b',random.randint(max(a,minb),maxb)))
k = int(cmdlinearg('k',random.randint(1,min(59,len(bin(b)[2:])))))

print(a,b,k)

