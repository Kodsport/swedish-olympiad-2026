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

seed = int(cmdlinearg('seed', sys.argv[-1] if len(sys.argv) > 1 and '=' not in sys.argv[-1] else 1337))
random.seed(seed)

n = 3
edges =[]

# Critical structure paths bridging the start, exit and rat
A1, A2 = 4, 5
B1, B2 = 6, 7
C1, C2, C3 = 8, 9, 10
n = 10

def add_e(u, v, t_id, orig_idx):
    edges.append((u, v, t_id, orig_idx))

add_e(1, A1, 'A', 1)
add_e(A1, A2, 'A', 2)
add_e(A2, 3, 'A', 3)

add_e(1, B1, 'B', 1)
add_e(B1, B2, 'B', 2)
add_e(B2, 3, 'B', 3)

add_e(1, C1, 'C', 1)
add_e(C1, C2, 'C', 2)
add_e(C2, C3, 'C', 3)
add_e(C3, 2, 'C', 4)

dummy_edges =[]

# Build the 1990 dummy edges in a depth-2 tree acting as a CPU sink
l1_nodes =[]
for i in range(45):
    n += 1
    add_e(1, n, 'D', len(dummy_edges) + 1)
    dummy_edges.append(len(edges) - 1)
    l1_nodes.append(n)

for i in range(1945):
    n += 1
    parent = l1_nodes[i % 45]
    add_e(parent, n, 'D', len(dummy_edges) + 1)
    dummy_edges.append(len(edges) - 1)

m = len(edges)
partner = [-1] * m

def pair_up(idx1, idx2):
    partner[idx1] = idx2
    partner[idx2] = idx1

def get_idx(t_id, orig_idx):
    for i, e in enumerate(edges):
        if e[2] == t_id and e[3] == orig_idx:
            return i

# Tie trap paths directly into the only path resolving to room 2
pair_up(get_idx('A', 1), get_idx('C', 1))
pair_up(get_idx('A', 2), get_idx('C', 2))
pair_up(get_idx('B', 1), get_idx('C', 3))
pair_up(get_idx('B', 2), get_idx('C', 4))

# THE UNIQUE SAFE PAIR 
pair_up(get_idx('A', 3), dummy_edges[0])
pair_up(get_idx('B', 3), dummy_edges[1])

# Make everything else a time-sink loop 
for i in range(2, len(dummy_edges), 2):
    pair_up(dummy_edges[i], dummy_edges[i+1])

# Thorough node and edge shuffling to fully fragment cache locality
nodes = list(range(4, n + 1))
random.shuffle(nodes)
mapping = {1: 1, 2: 2, 3: 3}
for i in range(4, n + 1):
    mapping[i] = nodes[i - 4]

edge_indices = list(range(m))
random.shuffle(edge_indices)
inv = {old: new for new, old in enumerate(edge_indices)}

new_edges = [None] * m
new_partner = [-1] * m

for i in range(m):
    u, v, _, _ = edges[i]
    new_edges[inv[i]] = (mapping[u], mapping[v])
    new_partner[inv[i]] = inv[partner[i]]

print(f"{n} {m}")
for i in range(m):
    u, v = new_edges[i]
    if random.choice([True, False]):
        u, v = v, u
    print(f"{u} {v} {new_partner[i] + 1}")