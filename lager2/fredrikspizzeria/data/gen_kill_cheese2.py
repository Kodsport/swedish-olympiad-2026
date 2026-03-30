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

# Read seed and configuration
seed = int(cmdlinearg('seed', sys.argv[-1] if len(sys.argv) > 1 and '=' not in sys.argv[-1] else 42))
random.seed(seed)

# L represents the length of the paths bridging room 1 and 3.
L = int(cmdlinearg('L', 499))
n = 3

edges = []
path_A_edges =[]
prev = 1
for i in range(L - 1):
    n += 1
    edges.append((prev, n))
    path_A_edges.append(len(edges) - 1)
    prev = n
edges.append((prev, 3))
path_A_edges.append(len(edges) - 1)

path_B_edges =[]
prev = 1
for i in range(L - 1):
    n += 1
    edges.append((prev, n))
    path_B_edges.append(len(edges) - 1)
    prev = n
edges.append((prev, 3))
path_B_edges.append(len(edges) - 1)

path_C_edges =[]
prev = 1
for i in range(2 * L + 1):
    n += 1
    edges.append((prev, n))
    path_C_edges.append(len(edges) - 1)
    prev = n
edges.append((prev, 2))
path_C_edges.append(len(edges) - 1)

# Add dummy leaves to node 1
n += 1
edges.append((1, n))
d1_edge = len(edges) - 1

n += 1
edges.append((1, n))
d2_edge = len(edges) - 1

m = len(edges)
partner = [-1] * m

def add_pair(e1, e2):
    partner[e1] = e2
    partner[e2] = e1

# This is the UNIQUE valid removal (e_A_0 paired with dummy D1, e_B_0 paired with D2)
add_pair(path_A_edges[0], d1_edge)
add_pair(path_B_edges[0], d2_edge)

# The rest of the edges on path A and B are hazardously paired to the path C to the exit
for i in range(1, L):
    add_pair(path_A_edges[i], path_C_edges[i - 1])
    add_pair(path_B_edges[i], path_C_edges[L - 1 + i - 1])

for i in range(2 * L - 2, 2 * L + 2, 2):
    add_pair(path_C_edges[i], path_C_edges[i+1])

# Shuffle node labels to stop edge-cases being solved by vertex index prediction 
# Node 1, 2, 3 must stay at their designated IDs
nodes = list(range(4, n + 1))
random.shuffle(nodes)
mapping = {1: 1, 2: 2, 3: 3}
for i in range(4, n + 1):
    mapping[i] = nodes[i - 4]

# Shuffle edge output orders
edge_indices = list(range(m))
random.shuffle(edge_indices)
inverse_edge_indices = {old: new for new, old in enumerate(edge_indices)}

new_edges = [None] * m
new_partner = [-1] * m

for i in range(m):
    u, v = edges[i]
    new_edges[inverse_edge_indices[i]] = (mapping[u], mapping[v])

for i in range(m):
    p = partner[i]
    new_partner[inverse_edge_indices[i]] = inverse_edge_indices[p]

print(f"{n} {m}")
for i in range(m):
    u, v = new_edges[i]
    if random.choice([True, False]):
        u, v = v, u
    print(f"{u} {v} {new_partner[i] + 1}")
