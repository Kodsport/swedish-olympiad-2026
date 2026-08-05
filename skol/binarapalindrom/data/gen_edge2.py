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

k = int(cmdlinearg('k'))

maxbits = int(cmdlinearg('maxbits',59))

if k == 1:
    # 1 is the only binary palindrome with exactly one one, so there is
    # nothing to randomize here (the generic construction below would give
    # an endpoint with three ones instead of one).
    a = b = 1
else:
    half = k//2

    a =  [1 for _ in range(half-1)] + [0]*randint(0,maxbits//2-half)
    b =  [1 for _ in range(half-1)] + [0]*randint(0,maxbits//2-half)
    random.shuffle(a)
    random.shuffle(b)

    a = [1] + a
    b = [1] + b


    if k%2:
        a = a + [1] + a[::-1]
    else:
        a = a + [0]*randint(0,1) +  a[::-1]

    if k%2:
        b = b + [1] + b[::-1]
    else:
        b = b + [0]*randint(0,1) + b[::-1]

    a = int("".join(map(str,a)),2)
    b = int("".join(map(str,b)),2)




if int(cmdlinearg("shift",0)):
    a += random.randint(-1,1)
    b += random.randint(-1,1)

a = max(a,1)
b = max(b,1)

if b < a:
    a,b = b,a

print(a,b,k)
