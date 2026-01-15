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
a_min = int(cmdlinearg('amin', 0))
a_max = int(cmdlinearg('amax', 10**9))
mode = cmdlinearg('mode', 'random')

if mode == "sub1":
    vals = [random.randint(a_min,a_max-n) for _ in range(n+1)]
    vals.sort()
    v = vals[0]
    a = vals[1:]
    for i in range(n):
        a[i] += i+1

elif mode == "sub2":
    a = [random.randint(a_min,a_max-n+1) for _ in range(n)]
    a.sort()
    a = a[::-1]
    for i in range(n):
        a[n-i-1] += i
    v = random.randint(a_min, a_max)

else:
    a = [random.randint(a_min,a_max) for _ in range(n)]
    v = random.randint(a_min, a_max)


print(n,v)
print(*a)

