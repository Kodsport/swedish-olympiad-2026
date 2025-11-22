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

nums = [randint(r-1, min(10**9, (r-1)*3)) for i in range(c)]
nums[random.randint(0,c-1)]=(r-1)+random.randint(1,20)
nums[random.randint(0,c-1)]=(r-1)+random.randint(1,20)
nums[random.randint(0,c-1)]=(r-1)+random.randint(1,20)
nums[random.randint(0,c-1)]=(r-1)+random.randint(1,20)
print(*(nums))

print(randint(0,2),randint(0,r-1))
print(randint(c-3,c-1),randint(r-5,r-2))
