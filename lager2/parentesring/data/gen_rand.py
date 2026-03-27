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
n = int(cmdlinearg('n', random.randint(1,10)))
a = int(cmdlinearg('a', random.randint(0,10)))
b = int(cmdlinearg('b', random.randint(0,10)))

mode = cmdlinearg("mode")
if mode == 'rand':
    s = [random.choice("()") for _ in range(n)]
    
elif mode == 'equal':
    assert(n%2 == 0)
    s = ["("]*(n//2) + [")"]*(n//2)
    random.shuffle(s)

elif mode == 'sorted':
    swaps = int(cmdlinearg('swaps',0))
    l = int(cmdlinearg('l', n//2))
    r = int(cmdlinearg('r', (n+1)//2))
    
    s = ["("]*(l) + [")"]*(r)

    rev = int(cmdlinearg('rev',0))

    if rev:
        s = s[::-1]
    
    for _ in range(swaps):
        i,j = random.randint(0,n-1), random.randint(0,n-1)
        s[i],s[j] = s[j],s[i]
        

else:
    assert(0), "mode not implemented"
print(n,a,b)
print("".join(s))
