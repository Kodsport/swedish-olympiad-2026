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
k = int(cmdlinearg('k'))
mode = cmdlinearg('mode')

people = []

if mode == "distinct_wide":
    # All 2K endpoints distinct, intervals spanning most of [1, N].
    # Pair smallest s with largest t to maximize span.
    # Breaks O(NK): 2K distinct endpoints * N sweep = TLE.
    all_positions = random.sample(range(1, n+1), 2*k)
    all_positions.sort()
    for i in range(k):
        s = all_positions[i]
        t = all_positions[2*k - 1 - i]
        people.append((s, t))

elif mode == "clustered_wide":
    # Endpoints clustered near 1 and near N, all distinct.
    # Every interval is nearly length N.
    lo_positions = random.sample(range(1, min(n, 4*k)), k)
    hi_positions = random.sample(range(max(1, n - 4*k + 1), n+1), k)
    for i in range(k):
        people.append((lo_positions[i], hi_positions[i]))

elif mode == "spread_long":
    # Evenly spread endpoints, each interval has length ~N/2.
    step = max(1, n // (2*k))
    positions = list(range(1, n+1, step))[:2*k]
    if len(positions) < 2*k:
        positions = random.sample(range(1, n+1), 2*k)
    positions.sort()
    half = len(positions) // 2
    for i in range(k):
        s = positions[i]
        t = positions[half + i] if half + i < len(positions) else positions[-1]
        people.append((s, t))

else:
    assert False, f"unknown mode: {mode}"

random.shuffle(people)

print(n, k)
for s, t in people:
    print(s, t)
