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
parts = int(cmdlinearg('parts'))
k = int(cmdlinearg('k'))
ctype = cmdlinearg('type')
extra_cycles = int(cmdlinearg('extra-edges', '0'))

assert k >= 3
assert parts >= 1
assert ctype in ('vertex', 'edge')

class UnionFind:
    def __init__(self, n):
        self.parent = list(range(n))
    def find(self, i):
        if self.parent[i] == i:
            return i
        self.parent[i] = self.find(self.parent[i])
        return self.parent[i]
    def union(self, i, j):
        root_i = self.find(i)
        root_j = self.find(j)
        if root_i != root_j:
            self.parent[root_i] = root_j

# Base Y-Tree has 3*parts + 1 cycles
num_cycles = 3 * parts + 1
C = []
curr_v = 0
for _ in range(num_cycles):
    C.append(list(range(curr_v, curr_v + k)))
    curr_v += k

uf = UnionFind(curr_v + extra_cycles * k)
bridges = []

def connect_cycles(u, port_u, v, port_v):
    if ctype == 'edge':
        # Edge mode: Every edge <= 1 cycle. Cycles share exactly ONE vertex.
        uf.union(C[u][port_u], C[v][port_v])
    else:
        # Vertex mode: Every vertex <= 1 cycle. Cycles cannot touch. Add a bridge.
        bridges.append((C[u][port_u], C[v][port_v]))

# Build the Y-tree of cycles
# Center cycle gets 3 distinct ports to ensure paths travel through its edges
ports_center = [0, k // 3, 2 * k // 3]

for b in range(3):
    prev_node = 0
    prev_port = ports_center[b]
    
    for j in range(1, parts + 1):
        curr_node = b * parts + j
        # Connect previous cycle to port 0 of the current cycle
        connect_cycles(prev_node, prev_port, curr_node, 0)
        
        prev_node = curr_node
        # Exit port for the next connection is halfway across the cycle
        prev_port = k // 2

# Add random extra cycles to the graph
for i in range(extra_cycles):
    new_c = list(range(curr_v, curr_v + k))
    curr_v += k
    C.append(new_c)
    
    target_node = random.randint(0, len(C) - 2)
    target_port = random.randint(0, k - 1)
    
    # Connect the new cycle to a random existing cycle
    connect_cycles(target_node, target_port, len(C) - 1, 0)

# Collect all raw edges (internal cycle edges + bridges)
edges = []
for c in C:
    for i in range(k):
        edges.append((c[i], c[(i + 1) % k]))
        
edges.extend(bridges)

# Deduplicate and remap edges post-merge
merged_edges = set()
for u, v in edges:
    cu, cv = uf.find(u), uf.find(v)
    if cu != cv:
        if cu > cv: cu, cv = cv, cu
        merged_edges.add((cu, cv))

# Identify the three furthest leaf vertices and pin them to canonical IDs 0, 1, 2
l1_root = uf.find(C[parts][k // 2])
l2_root = uf.find(C[2 * parts][k // 2])
l3_root = uf.find(C[3 * parts][k // 2])

canonical = {}
canonical[l1_root] = 0
canonical[l2_root] = 1
canonical[l3_root] = 2

# Relabel all remaining vertices uniformly
curr_id = 3
unique_roots = set()
for i in range(curr_v):
    root = uf.find(i)
    if root not in canonical:
        unique_roots.add(root)

inner_labels = list(range(3, 3 + len(unique_roots)))
random.shuffle(inner_labels)

inner_idx = 0
for root in unique_roots:
    canonical[root] = inner_labels[inner_idx]
    inner_idx += 1
    curr_id += 1

# Apply final mapping to edges
final_edges = []
for cu, cv in merged_edges:
    final_edges.append((canonical[cu], canonical[cv]))

# Ensure even edge count parity
if len(final_edges) % 2 == 1:
    final_edges.append((0, curr_id))
    curr_id += 1

m = len(final_edges)

# Apply pairing logic
pairing = list(range(m))
random.shuffle(pairing)

partner = [-1] * m
for i in range(0, m, 2):
    partner[pairing[i]] = pairing[i+1]
    partner[pairing[i+1]] = pairing[i]

# Output
print(curr_id, m)
for i, (u, v) in enumerate(final_edges):
    print(u + 1, v + 1, partner[i] + 1)
