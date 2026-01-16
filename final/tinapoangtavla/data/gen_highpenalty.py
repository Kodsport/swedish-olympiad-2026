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
p = int(cmdlinearg('p', 2))
h = int(cmdlinearg('h', 200000))
f = int(cmdlinearg('f', 200000))

#mode = cmdlinearg('mode')

from heapq import *

AC = 'AC'
WA = 'WA'

print(n, p, h, f)
nxtsolve = [1]*(n+1)

firstbatch = []
for i in range(n-1):
    while len(firstbatch) < h:
        
        if random.randint(0,int(cmdlinearg('prob', 1))) == 0:
            firstbatch.append((i+1,1,AC))
            nxtsolve[i+1] = 2
            break

        else:
            firstbatch.append((i+1,1,WA))

while len(firstbatch) < h:
    firstbatch += [(n,1,WA) for _ in range(h-len(firstbatch)-1)] + [(n,1,AC)]
    nxtsolve[n] = 2

d = int(cmdlinearg('d', 1))
secondbatch = []

for i in range(d-1):
    while 1:
        ind = random.randint(1,n)
        if nxtsolve[ind] > p:
            continue
        break

    secondbatch += [(ind,nxtsolve[ind],WA) for _ in range((f-1)//d)] + [(ind,nxtsolve[ind],AC)]
    nxtsolve[ind] += 1

while len(secondbatch) < f:
    while 1:
        ind = random.randint(1,n)
        if nxtsolve[ind] > p:
            continue
        break
    
    secondbatch += [(ind,nxtsolve[ind],WA) for _ in range(f-len(secondbatch)-1)] + [(ind,nxtsolve[ind],AC)]
    nxtsolve[ind] += 1



for x,y,z in firstbatch:
    print(x,y,z)
for x,y,z in secondbatch:
    print(x,y,z)

