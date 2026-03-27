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

def no_short(P):
    for i in range(len(P)):
        if P[P[i]] == i:
            return False
    return True

def to_permutation(partition, shuffle=1):
    n = sum(partition)
    cyc = []
    start = 0
    order = list(range(n))
    if shuffle == 1:
        random.shuffle(order)
    ans = list(range(n))

    for p in partition:
        c = []
        for i in range(start, start+p):
            c.append(order[i])
        cyc.append(c)
        start += p 
    
    for c in cyc:
        for i in range(len(c)):
            ans[c[i]] = c[(i+1)%len(c)]
    
    return ans

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n', 1))

mode = cmdlinearg('mode', 'random')
P = []

if mode == "random":
    P = list(range(n))
    random.shuffle(P)

elif mode == "identity":
    P = list(range(n))

elif mode == "one_cycle":
    P = list(range(n))
    for i in range(n):
        P[i] = (i+1)%n

elif mode == "random_no_short":
    P = list(range(n))
    random.shuffle(P)
    while not no_short(P):
        random.shuffle(P)

elif mode == "partition":
    part = eval(cmdlinearg('part', '[]'))
    P = to_permutation(part)

elif mode == "p23":
    assert n%3 == 2
    part = [3]*(n // 3) + [2]
    P = to_permutation(part, 0)

elif mode == "p23_reverse":
    assert n%3 == 2
    part = [2] + [3]*(n // 3)
    P = to_permutation(part, 0)


for i in range(len(P)):
    P[i] += 1

print(len(P))
print(*P)
