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

base = int(cmdlinearg('base',2))
mode = cmdlinearg('mode')

A = [base,base//2,base//2,base]
B = [base//2]*6

if mode == 'ver1':
    maxn = int(cmdlinearg('maxn'))




    cur = []

    while len(cur) + 6 < maxn:
        if random.randint(0,1):
            cur += A
        else:
            cur += B
elif mode == 'ver2':
    mult = int(cmdlinearg('mult'))

    cur = []

    for _ in range(mult):
        if random.randint(0,1):
            cur += A
        else:
            cur += B
elif mode == 'ver3':
    mult = int(cmdlinearg('mult'))

    cur = []

    for _ in range(mult):
            cur += A
else:
    assert 0


shift = int(cmdlinearg('shift',0))

cur = cur[shift:] + cur[:shift]


print(len(cur))
print(*cur)

