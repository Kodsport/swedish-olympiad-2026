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

def linearSieve(n):
    LPF = [0]*n
    pr = []

    for i in range(2, n):
        if LPF[i] == 0:
            LPF[i] = i
            pr.append(i)
        
        for p in pr:
            if p*i >= n: break
            LPF[i * p] = p
            if LPF[i] == p: break 
    
    return LPF, pr

LPF, primes = linearSieve(10**3)


random.seed(int(cmdlinearg('seed', sys.argv[-1])))

n = int(cmdlinearg('n', 1))
x = int(cmdlinearg('x', 1))

a = primes[:n//2]
random.shuffle(a)

b = [a[i]*a[(i+1)%len(a)] for i in range(len(a))]

out = a + b

random.shuffle(out)

print(len(out),x)
print(*out)
