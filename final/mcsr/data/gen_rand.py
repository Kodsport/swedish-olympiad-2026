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
s = int(cmdlinearg('s', random.randint(8*n+1,10**9)))

mint = int(cmdlinearg('mint', 1))
maxt = int(cmdlinearg('maxt', s*7//8))

mode = cmdlinearg('mode', "rand")

if mode=="rand":
    if s >= 2000000:
        pos = set()
        while len(pos) < n:
            pos.add(random.randint(1,(s-1)//8))
        pos = list(pos)
        pos.sort()
    else:
        choices = [i//8 for i in range(8,s,8)]
        pos = random.sample(choices,n)
        pos.sort()


    tim = [random.randint(mint,maxt) for _ in range(n)]

else:
    assert 0

print(n,s)
for x,t in zip(pos,tim):
    print(x,t)

