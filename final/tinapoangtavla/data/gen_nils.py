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
p = int(cmdlinearg('p'))
h = int(cmdlinearg('h'))
f = int(cmdlinearg('f'))


submissions = []

for i in range(h):
    person = random.randint(1,n)
    problem = random.randint(1,p)
    submissions.append((person, problem, "WA"))

siz = [0] * n
rem = f
for i in range(n):
    g = i%p+1
    if g > rem:
        g = rem
    rem -= g
    siz[i] = g
siz.sort()
siz = siz[::-1]

subs2 = []

for i in range(n):
    for j in range(0,siz[i]):
        subs2.append((i+1, j+1, "AC"))

b = int(cmdlinearg('shuffle', 0))
if b == 1:
    random.shuffle(subs2)

submissions += subs2

print(n, p, h, f)

for s in submissions:
    print(*s)

