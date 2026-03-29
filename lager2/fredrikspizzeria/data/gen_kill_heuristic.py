#!/usr/bin/python3
import sys

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        sys.exit(1)
    return default

# K = 50000 ensures 2.5 * 10^9 operations for a naive nested loop
K = int(cmdlinearg('k', '50000'))

n = 3
edges = []
pairings = []

def add_edge(u, v):
    edges.append((u, v))
    pairings.append(-1)
    return len(edges) - 1

def pair_edges(e1, e2):
    pairings[e1] = e2
    pairings[e2] = e1

# Build path 1 -> 2 as a chain of K cycles
prev = 1
c12_left = []
c12_right = []
for i in range(K):
    n += 1; a = n
    n += 1; b = n
    
    add_edge(prev, a)
    c12_left.append(add_edge(a, b))
    c12_right.append(add_edge(prev, b))
    prev = b
add_edge(prev, 2)

# Build path 1 -> 3 as ONE giant cycle
# Branch A
prev = 1
branch_a = []
for i in range(K):
    n += 1; a = n
    branch_a.append(add_edge(prev, a))
    prev = a
end_3 = prev

# Branch B
prev = 1
branch_b = []
for i in range(K):
    n += 1; b = n
    branch_b.append(add_edge(prev, b))
    prev = b

# Close the giant cycle and attach to 3
add_edge(end_3, 3)
add_edge(prev, 3)

# Pair them up to create exactly 1 valid solution at the very end.
# A[i] pairs with c12_left[i]. B[i] pairs with c12_right[i].
# If a solver picks A[i] and B[i], it breaks both sides of cycle i on 1->2.
for i in range(K - 1):
    pair_edges(branch_a[i], c12_left[i])
    pair_edges(branch_b[i], c12_right[i])

# The last pair A[K-1] and B[K-1] are paired with dummy edges so they are VALID.
n += 1; dummy1 = n
n += 1; dummy2 = n
d_e1 = add_edge(2, dummy1)
d_e2 = add_edge(2, dummy2)

pair_edges(branch_a[-1], d_e1)
pair_edges(branch_b[-1], d_e2)

# Pair up remaining unpaired edges (like entry/exit edges of 1->2 cycles)
unpaired = [i for i in range(len(edges)) if pairings[i] == -1]
if len(unpaired) % 2 == 1:
    n += 1
    unpaired.append(add_edge(2, n))

for i in range(0, len(unpaired), 2):
    pair_edges(unpaired[i], unpaired[i+1])

print(f"{n} {len(edges)}")
for i in range(len(edges)):
    print(f"{edges[i][0]} {edges[i][1]} {pairings[i] + 1}")