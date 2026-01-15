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
maxval = int(cmdlinearg('m', 10**9))
mode = cmdlinearg("mode")
if mode=="rand":
    nums = [randint(1, maxval) for i in range(n)]
elif mode=="evil":
    S = 6983776800  # highly composite number
    S = min(S,maxval+n-1)
    nums = [1] * (n - 1)
    nums.append(S - (n - 1))
elif mode=="pow2":
    chosen_n = 1
    while chosen_n*2 < n:
        chosen_n *= 2
    nums = [1] * chosen_n
elif mode == '1and2':
    base = int(cmdlinearg('base',1))

    nums = [[base,base<<1][random.randint(0,1)] for _ in range(n)]

else:
    assert 0

print(len(nums))
print(*nums)
