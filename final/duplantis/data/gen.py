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
v = int(cmdlinearg('v', random.randint(a_min,a_max)))

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

elif mode == "sub3":
    n = 2
    a1 = int(cmdlinearg('a1', random.randint(a_min,a_max)))
    a2 = int(cmdlinearg('a2', random.randint(a_min,a_max)))
    a = [a1, a2]

elif mode == "simulate":
    p = float(cmdlinearg('p', 0.5))
    v2 = v
    a = []
    for i in range(n):
        w = v2 + random.randint(-10,10)
        if random.random() > p:
            w = random.randint(a_min, a_max)
        if w > v2:
            v2 += 1
        a.append(w)
        

else:
    a = [random.randint(a_min,a_max) for _ in range(n)]


print(n,v)
print(*a)

