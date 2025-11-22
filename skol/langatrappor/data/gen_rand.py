#!/usr/bin/python3

import sys
import random
from random import randint
import math

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))

r = int(cmdlinearg('r'))
c = int(cmdlinearg('c'))
mode = cmdlinearg('mode')
print(r, c)

if mode=='3x':
    print(*(randint(r-1, (r-1)*3) for i in range(c)))
elif mode=='exp':
    base = float(cmdlinearg('base'))
    lo = int(1+math.log(r)/math.log(base))
    hi = int(math.log(1e9)/math.log(base))
    print(*(randint(r-1, max(r-1, int(base**random.randint(lo,hi)))) for i in range(c)))
else:
    assert 0

print(randint(0,c-1),randint(0,r-1))
print(randint(0,c-1),randint(0,r-1))
