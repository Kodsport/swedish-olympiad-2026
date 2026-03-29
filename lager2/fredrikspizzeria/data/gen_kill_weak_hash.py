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

# Path 1 -> 2 cycles to generate constraints
c12_edges = []
prev = 1
for _ in range(10):
    n += 1; a = n
    n += 1; b = n
    add_edge(prev, a)
    c12_edges.append(add_edge(a, b)) # Left side 1->2
    c12_edges.append(add_edge(prev, b)) # Right side 1->2
    prev = b
add_edge(prev, 2)

# 1 -> 3 Cycle
prev = 1
branch_a = []
for _ in range(8):
    n += 1
    branch_a.append(add_edge(prev, n))
    prev = n
cycle_exit = prev

prev = 1
branch_b = []
for _ in range(8):
    n += 1
    branch_b.append(add_edge(prev, n))
    prev = n

add_edge(cycle_exit, 3)
add_edge(prev, 3)

# Deliberate indices configuration to force sum/XOR collisions.
# We pair them such that the required states map to identical integers.
# Let valid pair be branch_a[0] and branch_b[7].
# Let invalid pair be branch_a[7] and branch_b[0].
# Both sum to the same index total and often XOR to similar bit patterns depending on layout.

for i in range(1, 7):
    pair_edges(branch_a[i], c12_edges[i])
    pair_edges(branch_b[i], c12_edges[i+1])

# Invalid pair: damages both sides of a 1->2 cycle
pair_edges(branch_a[7], c12_edges[0]) 
pair_edges(branch_b[0], c12_edges[1]) # Assuming 0 and 1 are the same cycle in 1->2

# Valid pair: paired with dummies
n += 1; d1 = n
n += 1; d2 = n
e_d1 = add_edge(2, d1)
e_d2 = add_edge(2, d2)

pair_edges(branch_a[0], e_d1)
pair_edges(branch_b[7], e_d2)

unpaired = [i for i in range(len(edges)) if pairings[i] == -1]
if len(unpaired) % 2 != 0:
    n += 1
    unpaired.append(add_edge(2, n))
for i in range(0, len(unpaired), 2):
    pair_edges(unpaired[i], unpaired[i+1])

print(f"{n} {len(edges)}")
for i in range(len(edges)):
    print(f"{edges[i][0]} {edges[i][1]} {pairings[i] + 1}")