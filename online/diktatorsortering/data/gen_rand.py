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

n = int(cmdlinearg('n'))

mode = cmdlinearg("mode")
print(n)
nums = []
if mode=='random':
    maxv = int(cmdlinearg('maxv'))
    for i in range(n):
        nums.append(randint(1,maxv))
elif mode=='almost_inc':
    swaps = randint(n//20, n//10)
    nums = [i+1 for i in range(n)]
    for i in range(swaps):
        i=randint(0,n-1)
        j=randint(0,n-1)
        nums[i],nums[j]=nums[j],nums[i]
elif mode=='almost_dec':
    swaps = int(n * float(cmdlinearg('p_swaps', random.uniform(0.05,0.1))))
    nums = [n-i for i in range(n)]
    for i in range(swaps):
        i=randint(0,n-1)
        j=randint(0,n-1)
        nums[i],nums[j]=nums[j],nums[i]
else:
    assert 0
print(*nums)
