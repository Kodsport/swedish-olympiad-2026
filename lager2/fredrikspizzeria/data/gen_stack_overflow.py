#!/usr/bin/python3
import sys

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        sys.exit(1)
    return default

N = int(cmdlinearg('n', '200000'))
edges = []
pairings = [-1] * (N - 1)

# Linear chain: 1 -> 4 -> 5 -> ... -> N -> 3 -> 2
# Ensures DFS travels the maximum distance before resolving.
sequence = [1] + list(range(4, N + 1)) + [3, 2]

for i in range(len(sequence) - 1):
    edges.append((sequence[i], sequence[i+1]))

# Dummy pairings (sequential)
for i in range(0, len(edges) - 1, 2):
    pairings[i] = i + 1
    pairings[i+1] = i

if len(edges) % 2 != 0:
    # Append one dummy node to make edge count even
    edges.append((2, N + 1))
    pairings.append(len(edges) - 2) # Arbitrary safe pair
    pairings[len(edges) - 2] = len(edges) - 1
    N += 1

print(f"{N} {len(edges)}")
for i in range(len(edges)):
    print(f"{edges[i][0]} {edges[i][1]} {pairings[i] + 1}")