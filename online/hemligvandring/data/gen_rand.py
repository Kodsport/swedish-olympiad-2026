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

n = int(cmdlinearg('n', 75))
T = int(cmdlinearg('T', 1))

x1 = int(cmdlinearg('x1',random.randint(0,(n-3)//2)*2 + 1))
y1 = int(cmdlinearg('y1',random.randint(0,(n-3)//2)*2 + 1))
x2 = int(cmdlinearg('x2',random.randint(0,(n-3)//2)*2 + 1))
y2 = int(cmdlinearg('y2',random.randint(0,(n-3)//2)*2 + 1))

while x1 == x2:
    x2 = random.randint(0,(n-3)//2)*2 + 1

while y1 == y2:
    y2 = random.randint(0,(n-3)//2)*2 + 1

print(n,T)
print(x1,y1,x2,y2)
