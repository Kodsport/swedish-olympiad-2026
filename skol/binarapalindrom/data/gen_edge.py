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

maxbits = int(cmdlinearg('maxbits', 59))

a = [1] + [random.randint(0,1) for _ in range(random.randint(max(1,maxbits//2),maxbits)//2)]
b = [1] + [random.randint(0,1) for _ in range(random.randint(max(1,maxbits//2),maxbits)//2)]

if random.randint(0,1):
    a = a + [random.randint(0,1)] + a[::-1]
else:
    a = a + a[::-1]

if random.randint(0,1):
    b = b + [random.randint(0,1)] + b[::-1]
else:
    b = b + b[::-1]

a = int("".join(map(str,a)),2)
b = int("".join(map(str,b)),2)



k = int(cmdlinearg('k',[bin(a).count("1"),bin(b).count("1")][random.randint(0,1)]))

if int(cmdlinearg("shift",0)):
    a += random.randint(-1,1)
    b += random.randint(-1,1)

a = max(a,1)
b = max(b,1)

if b < a:
    a,b = b,a

print(a,b,k)
