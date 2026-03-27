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
n = int(cmdlinearg('n'))
k = int(cmdlinearg('k', random.randint(1, n)))
mode = cmdlinearg('mode')

if mode=="random":
    people = [(random.randint(1, n), random.randint(1, n)) for _ in range(k)]
elif mode=="long_intervals":
    people = [(random.randint(1, 10), random.randint(n-10, n)) for _ in range(k)]
elif mode=="decoys":
    decoys = int(cmdlinearg('decoys'))
    center = n // 5
    people = []
    for i in range(decoys):
        s = max(1, center - decoys // 2 + i)
        t = min(n, s + 2)
        people.append((s, t))

    for i in range(k - decoys):
        people.append((1, n))
elif mode=="bimodal":
    d1 = int(cmdlinearg('d1'))
    d2 = int(cmdlinearg('d2'))
    c1 = int(cmdlinearg('c1'))

    c2 = k - c1
    people = []
    mid = n // 2
    for i in range(c1):
        s = max(1, mid - d1 // 2 + random.randint(-2, 2))
        t = min(n, s + d1)
        people.append((s, t))

    for i in range(c2):
        s = max(1, random.randint(1, max(1, n - d2)))
        t = min(n, s + d2)
        people.append((s, t))
elif mode=="distinct":
    assert k+1 <= n
    people = []
    for i in range(k):
        people.append((i+1, i+2))
elif mode=="concentrated":
    g1 = int(cmdlinearg('g1'))
    g2 = int(cmdlinearg('g2'))
    l_group = random.sample(range(1, n+1), g1)
    r_group = random.sample(range(1, n+1), g2)
    people = []
    for i in range(k):
        s = random.choice(l_group)
        t = random.choice(r_group)
        people.append((s, t))
else:
    assert False, "unknown mode"

random.shuffle(people)

print(n, k)
for s,t in people:
    print(s, t)
