#!/usr/bin/python3

import sys
import random

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n', 1))
x = int(cmdlinearg('x', 1))
maxx = int(cmdlinearg('maxx', 10**6))
a1 = int(cmdlinearg('a1', -1))
u = int(cmdlinearg('u', 1000))
mode = cmdlinearg('mode', 'random')

u = min(u, maxx)

A = []
diff = random.sample(range(1, maxx+1), u)

if mode == "random":
    for _ in range(n):
        A.append(diff[random.randrange(len(diff))])

if a1 != -1:
    A[0] = a1

print(len(A),x)
print(*A)



