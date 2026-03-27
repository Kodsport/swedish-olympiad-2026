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
a = int(cmdlinearg('a', random.randint(1,10)))
b = int(cmdlinearg('b', random.randint(0,10)))

mode = cmdlinearg("mode")
if mode == 'rand':
    s = []
    
    cnt = 0
    arr = []
    
    p = float(cmdlinearg('p', 0.5))
    
    while len(s)+len(arr)+cnt+2 <= n:
        if cnt == 0:
            if arr:
                s += arr
            arr = []
            arr.append("(")
            cnt += 1
            continue
        
        if random.random() < p:
            arr.append("(")
            cnt += 1
        else:
            arr.append(")")
            cnt -= 1
    
    s += arr
    s += [")"]*cnt

elif mode == 'fractal':
    cnt = 2
    s = "()"
    while cnt*2+2 <= n:
        s = "(" + s + s + ")"
        cnt = cnt*2+2
    s = [*s]

else:
    assert(0), "mode not implemented"

extra = int(cmdlinearg('k', 0))
if extra:
    n = len(s)
    inserts = []
    for _ in range(extra):
        inserts.append((random.randint(0,n),random.choice("()")))
    inserts.sort()

    olds = [(i,x) for i,x in enumerate(s)]

    s = []
    olds.reverse()
    inserts.reverse()
    while olds and inserts:
        if olds[-1][0] <= inserts[-1][0]:
            s.append(olds.pop()[1])
        else:
            s.append(inserts.pop()[1])
    
    while olds:
        s.append(olds.pop()[1])

    while inserts:
        s.append(inserts.pop()[1])


spin = int(cmdlinearg('spin', 0))
s = s[spin:] + s[:spin]

n = len(s)
print(n,a,b)
print("".join(s))
