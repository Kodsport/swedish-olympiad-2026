#!/usr/bin/python3

import sys

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

P = int(cmdlinearg('p'))
mode = cmdlinearg('mode', 'vertex')  # 'vertex' (vertex-cactus) or 'edge'

# Key idea:
# One big cycle C on the 1->2 path with P left and P+1 right edges.
# One giant cycle G on the 1->3 path with P+1 edges per branch.
# Pair all A-branch (except A[0]) with left-C edges: gives constraint (C, 1).
# Pair all B-branch with right-C edges: gives constraint (C, 0).
# Pair A[0] with a dummy edge: no constraint (the "escape" variable).
#
# The heuristic iterates a_vars x b_vars for G.
# All a_vars (except the last one, A[0]) have (C,1).
# All b_vars have (C,0).
# Every pair conflicts except those involving A[0].
# A[0] appears last in a_vars (due to DFS cycle_edges ordering).
# With P >= ~5, the heuristic's MAX_OPS=2500 is exhausted before reaching A[0].

n = 3  # nodes 1, 2, 3
edges = []
pairings = []

def add_edge(u, v):
    edges.append((u, v))
    pairings.append(-1)
    return len(edges) - 1

def pair_edges(e1, e2):
    pairings[e1] = e2
    pairings[e2] = e1

if mode == 'vertex':
    # Vertex-cactus: cycles don't share vertices.
    # Add bridge nodes u, v (for cycle C) and w, x (for cycle G).
    n += 1; u = n
    n += 1; v = n
    n += 1; w = n
    n += 1; x = n

    # Bridges (added first to control DFS order at node 1)
    bu = add_edge(1, u)   # on 1->2 path
    bv = add_edge(v, 2)   # on 1->2 path
    bw = add_edge(1, w)   # on 1->3 path
    bx = add_edge(x, 3)   # on 1->3 path

    # Pair bridges: cutting 1->3 bridge also cuts 1->2 bridge -> infeasible
    pair_edges(bu, bw)
    pair_edges(bv, bx)

    # Cycle C: left path u -> L_0 -> ... -> L_{P-2} -> v  (P edges)
    el = []
    prev = u
    for i in range(P - 1):
        n += 1
        el.append(add_edge(prev, n))
        prev = n
    el.append(add_edge(prev, v))

    # Cycle C: right path u -> R_0 -> ... -> R_{P-1} -> v  (P+1 edges)
    er = []
    prev = u
    for i in range(P):
        n += 1
        er.append(add_edge(prev, n))
        prev = n
    er.append(add_edge(prev, v))

    # Giant cycle G: branch A  w -> A_0 -> ... -> A_{P-1} -> x  (P+1 edges)
    ea = []
    prev = w
    for i in range(P):
        n += 1
        ea.append(add_edge(prev, n))
        prev = n
    ea.append(add_edge(prev, x))

    # Giant cycle G: branch B  w -> B_0 -> ... -> B_{P-1} -> x  (P+1 edges)
    eb = []
    prev = w
    for i in range(P):
        n += 1
        eb.append(add_edge(prev, n))
        prev = n
    eb.append(add_edge(prev, x))

    # Dummy edge
    n += 1
    ed1 = add_edge(2, n)

    # Pairings:
    # EA[1..P] <-> EL[0..P-1]  (all constrain cycle C)
    for i in range(P):
        pair_edges(ea[i + 1], el[i])

    # EB[0..P] <-> ER[0..P]  (all constrain cycle C)
    for i in range(P + 1):
        pair_edges(eb[i], er[i])

    # EA[0] <-> ED1  (no constraint - the escape variable)
    pair_edges(ea[0], ed1)

elif mode == 'edge':
    # Edge-cactus only (not vertex-cactus): node 1 shared between C and G.
    # Cycle C: left path 1 -> L_0 -> ... -> L_{P-2} -> 2  (P edges)
    el = []
    prev = 1
    for i in range(P - 1):
        n += 1
        el.append(add_edge(prev, n))
        prev = n
    el.append(add_edge(prev, 2))

    # Cycle C: right path 1 -> R_0 -> ... -> R_{P-1} -> 2  (P+1 edges)
    er = []
    prev = 1
    for i in range(P):
        n += 1
        er.append(add_edge(prev, n))
        prev = n
    er.append(add_edge(prev, 2))

    # Giant cycle G: branch A  1 -> A_0 -> ... -> A_{P-1} -> 3  (P+1 edges)
    ea = []
    prev = 1
    for i in range(P):
        n += 1
        ea.append(add_edge(prev, n))
        prev = n
    ea.append(add_edge(prev, 3))

    # Giant cycle G: branch B  1 -> B_0 -> ... -> B_{P-1} -> 3  (P+1 edges)
    eb = []
    prev = 1
    for i in range(P):
        n += 1
        eb.append(add_edge(prev, n))
        prev = n
    eb.append(add_edge(prev, 3))

    # Dummy edge
    n += 1
    ed1 = add_edge(2, n)

    # Pairings
    for i in range(P):
        pair_edges(ea[i + 1], el[i])
    for i in range(P + 1):
        pair_edges(eb[i], er[i])
    pair_edges(ea[0], ed1)

# Verify all edges paired
for i in range(len(edges)):
    assert pairings[i] != -1, f"Edge {i} unpaired"
    assert pairings[i] != i, f"Edge {i} paired with itself"
    assert pairings[pairings[i]] == i, f"Pairing not symmetric for edge {i}"

assert len(edges) % 2 == 0, f"Odd number of edges: {len(edges)}"

print(f"{n} {len(edges)}")
for i in range(len(edges)):
    print(f"{edges[i][0]} {edges[i][1]} {pairings[i] + 1}")
