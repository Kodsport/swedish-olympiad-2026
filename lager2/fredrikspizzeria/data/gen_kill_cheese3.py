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

seed = int(cmdlinearg('seed', sys.argv[-1] if len(sys.argv) > 1 and '=' not in sys.argv[-1] else 42))
random.seed(seed)

def get_hierarchical_order(num_edges):
    """ Outputs indices to build a maximally deep binomial tree for Union-by-Size DSU """
    edges = list(range(1, num_edges + 1))
    ordered =[]
    step = 2
    while True:
        level_edges =[]
        for i in range(step // 2 - 1, num_edges, step):
            level_edges.append(edges[i])
        if not level_edges:
            break
        ordered.extend(level_edges)
        step *= 2
        
    used = set(ordered)
    for e in edges:
        if e not in used:
            ordered.append(e)
    return ordered

# Nodes: 1 (start), 2 (exit), 3 (rat). N=2000, M=2000
# 1993 intermediate nodes for Path C
C_edges_orig = [(1, 4)]
for i in range(4, 1996):
    C_edges_orig.append((i, i + 1))
C_edges_orig.append((1996, 2))

# Order Path C hierarchically to nuke the DSU union-by-size flat tree
hier_indices = get_hierarchical_order(1994)

class EdgeObj:
    def __init__(self, u, v, type_id, orig_idx):
        self.u = u
        self.v = v
        self.type_id = type_id
        self.orig_idx = orig_idx

edge_objs =[]
for i in range(1994):
    edge_objs.append(EdgeObj(C_edges_orig[i][0], C_edges_orig[i][1], 'C', i))
edge_objs.append(EdgeObj(1, 1999, 'D', 1))
edge_objs.append(EdgeObj(1, 2000, 'D', 2))
edge_objs.append(EdgeObj(1, 1997, 'A', 1))
edge_objs.append(EdgeObj(1997, 3, 'A', 2))
edge_objs.append(EdgeObj(1, 1998, 'B', 1))
edge_objs.append(EdgeObj(1998, 3, 'B', 2))

hier_C = [edge_objs[idx - 1] for idx in hier_indices]
D_edges = [e for e in edge_objs if e.type_id == 'D']
AB_edges = [e for e in edge_objs if e.type_id in ('A', 'B')]

# Interleave the dummy edges to obscure the paired IDs
first_part = hier_C[:]
first_part.insert(random.randint(0, len(first_part)), D_edges[0])
first_part.insert(random.randint(0, len(first_part)), D_edges[1])

# Put Path A and Path B at the VERY END so DSU early exit never triggers!
final_edges = first_part + AB_edges

edge_to_idx = {e: i for i, e in enumerate(final_edges)}
partner = [-1] * 2000

def add_pair(e1, e2):
    idx1, idx2 = edge_to_idx[e1], edge_to_idx[e2]
    partner[idx1], partner[idx2] = idx2, idx1

def find_edge(type_id, orig_idx):
    for e in final_edges:
        if e.type_id == type_id and e.orig_idx == orig_idx:
            return e

# The UNIQUE correct double pair removal to isolate Room 3 
# (disables A1 and B1 but leaves all of Path C intact)
add_pair(find_edge('A', 1), find_edge('D', 1)) 
add_pair(find_edge('B', 1), find_edge('D', 2)) 

# Disabling A2 or B2 is a trap, it breaks Path C and disconnects the exit.
add_pair(find_edge('A', 2), find_edge('C', 0))
add_pair(find_edge('B', 2), find_edge('C', 1))

remaining_C =[find_edge('C', i) for i in range(2, 1994)]
for i in range(0, 1992, 2):
    add_pair(remaining_C[i], remaining_C[i+1])

# Shuffle node IDs to kill index/vertex prediction
nodes = list(range(4, 2001))
random.shuffle(nodes)
mapping = {1: 1, 2: 2, 3: 3}
for i in range(4, 2001):
    mapping[i] = nodes[i - 4]

print(f"2000 2000")
for i, e in enumerate(final_edges):
    u, v = mapping[e.u], mapping[e.v]
    if random.choice([True, False]):
        u, v = v, u
    print(f"{u} {v} {partner[i] + 1}")