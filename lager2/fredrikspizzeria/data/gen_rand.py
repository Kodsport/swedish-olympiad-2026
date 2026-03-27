#!/usr/bin/python3
# This generator sucks. Will have m=n+log(n) edges. Runs fast at least

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

def generate_cactus():
    components = [[i] for i in range(n)]

    edges = []
    while len(components) > 1:
        k = random.randint(2, len(components))

        representatives = []
        merged = []
        for _ in range(k):
            comp = components.pop()
            representatives.append(random.choice(comp))
            merged += comp

        if k == 2:
            edges.append((representatives[0], representatives[1]))
        else:
            for i in range(k):
                edges.append((representatives[i], representatives[(i+1) % k]))

        components.append(merged)

        random.shuffle(components)
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
