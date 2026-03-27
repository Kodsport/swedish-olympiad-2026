#!/usr/bin/python3

import sys
import random
import tree

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
n_cycles = int(cmdlinearg('cycles'))
assert n_cycles >= 1

def generate_cactus():
    nums = list(range(n))
    random.shuffle(nums)

    cuts = sorted(random.sample(range(1, n), n_cycles - 1))
    parts = []
    
    prev = 0
    for c in cuts:
        parts.append(nums[prev:c])
        prev = c
    parts.append(nums[prev:])
    
    random.shuffle(parts)

    edges = []
    for i in range(len(parts)-1):
        u = random.choice(parts[i])
        v = random.choice(parts[i+1])
        edges.append((u, v))
    
    for part in parts:
        if len(part) == 2:
            edges.append((part[0], part[1]))
        elif len(part) > 2:
            for i in range(len(part)):
                edges.append((part[i], part[(i+1) % len(part)]))
    
    return edges

edges = generate_cactus()
while len(edges) % 2 == 1:
    edges = generate_cactus()

edges = tree.shuffle_labels(edges, n=n)
assert len(edges) % 2 == 0
m = len(edges)

pairing = list(range(m))
random.shuffle(pairing)

partner = [-1]*m
for i in range(0, m, 2):
    partner[pairing[i]] = pairing[i+1]
    partner[pairing[i+1]] = pairing[i]

print(n, m)
for i, (u, v) in enumerate(edges):
    print(u+1, v+1, partner[i]+1)
