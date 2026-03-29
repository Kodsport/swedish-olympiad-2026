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
    # Edge cactus: adjacent parts share a vertex (not an edge)
    # so each edge belongs to at most one cycle, but vertices may be in multiple cycles.
    # Total vertex slots = n + (n_cycles - 1), since each connection shares one vertex.
    total_slots = n + n_cycles - 1

    nums = list(range(n))
    random.shuffle(nums)

    if n_cycles == 1:
        sizes = [total_slots]
    else:
        cuts = sorted(random.sample(range(1, total_slots), n_cycles - 1))
        sizes = []
        prev = 0
        for c in cuts:
            sizes.append(c - prev)
            prev = c
        sizes.append(total_slots - prev)

    random.shuffle(sizes)

    parts = []
    idx = 0
    for i in range(n_cycles):
        if i == 0:
            part = nums[idx:idx + sizes[i]]
            idx += sizes[i]
        else:
            shared = random.choice(parts[-1])
            new_count = sizes[i] - 1
            part = [shared] + nums[idx:idx + new_count]
            idx += new_count
        parts.append(part)

    edges = []
    for part in parts:
        if len(part) == 2:
            edges.append((part[0], part[1]))
        elif len(part) > 2:
            for j in range(len(part)):
                edges.append((part[j], part[(j+1) % len(part)]))

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
