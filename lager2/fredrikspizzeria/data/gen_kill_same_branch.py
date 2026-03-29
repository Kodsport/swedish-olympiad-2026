#!/usr/bin/python3
import sys

def add_edge(u, v):
    edges.append((u, v))
    pairings.append(-1)
    return len(edges) - 1

def pair_edges(e1, e2):
    pairings[e1] = e2
    pairings[e2] = e1

n = 3
edges = []
pairings = []

K = 1000 # Edges per branch

# Path 1 -> 2: A chain of K bridges
prev = 1
path12_edges = []
for _ in range(K):
    n += 1
    path12_edges.append(add_edge(prev, n))
    prev = n
add_edge(prev, 2)

# Path 1 -> 3: A single cycle
# Left branch (Safe to cut)
prev = 1
left_branch = []
for _ in range(K):
    n += 1
    left_branch.append(add_edge(prev, n))
    prev = n
cycle_exit = prev

# Right branch (Forbidden to cut)
prev = 1
right_branch = []
for _ in range(K):
    n += 1
    right_branch.append(add_edge(prev, n))
    prev = n

# Connect cycle to 3
add_edge(cycle_exit, 3)
add_edge(prev, 3)

# Pair Right branch with 1->2 bridges (Forbidden)
for i in range(K):
    pair_edges(right_branch[i], path12_edges[i])

# Pair Left branch with dummy safe edges attached to node 2
for i in range(K):
    n += 1; d1 = n
    d_edge = add_edge(2, d1)
    pair_edges(left_branch[i], d_edge)

# Pair remaining unpaired edges safely
unpaired = [i for i in range(len(edges)) if pairings[i] == -1]
if len(unpaired) % 2 != 0:
    n += 1
    unpaired.append(add_edge(2, n))

for i in range(0, len(unpaired), 2):
    pair_edges(unpaired[i], unpaired[i+1])

print(f"{n} {len(edges)}")
for i in range(len(edges)):
    print(f"{edges[i][0]} {edges[i][1]} {pairings[i] + 1}")