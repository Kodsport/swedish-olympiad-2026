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

start = int(cmdlinearg('start',0))

# The answer should be [d]*m

m = int(cmdlinearg('m'))
d = int(cmdlinearg('d',10**11//m))

if start:
    m -= 1

r = int(cmdlinearg('r', 2))

a = []
for _ in range(m):

    lim = int(cmdlinearg('lim',1))
    
    def div(n,k):
        if n <= lim:
            return [n]
        
        if k:
            s = random.randint(1,n-1)
            return div(s,k-1)  + div(n-s,k-1)
        
        if random.randint(0,1):
            s = random.randint(1,n-1)
            return div(s,0) + div(n-s,0)
        
        return [n]
    
    
    a += div(d,r)

if start:
    a = [d]+a

shift = int(cmdlinearg('shift',0))
a = a[shift:] + a[:shift]

print(len(a))
print(*a)

