#!/usr/bin/python3

"""
Kills quadratic solutions (joshua_m2.cpp, joshua_nicecactus_m2.cpp).

Construction: A big cycle (2K nodes) shared by both the escape path (path1: room1->room2)
and the rat path (path2: room1->room3), plus 2K triangles on the escape path whose edges
are paired with the big cycle's edges. This gives K distinct cut-signatures on each side
of the path2-cycle, forcing K^2 pair comparisons in the nested loop, all of which are "bad"
(they always damage the big cycle on both sides). The answer is always "Nej".

The graph is a vertex cactus (each node in at most one cycle), so this also works for group 6.

Node layout (1-indexed):
  1 = room 1 (Elsa), 2 = room 2 (exit), 3 = room 3 (rat) = B[K]
  Big cycle: B[0..2K-1], path1&2 use top half B[0]->...->B[K]
  Triangles: 2K triangles, each with 3 unique nodes
  Bridges connect room1->B[0], B[K]->tri chain->room2

Edge pairings ensure:
  lside_cuts = K elements {(big_cycle, 1), (tri_i, 0)}
  rside_cuts = K elements {(big_cycle, 0), (tri_{K+j}, 1)}
  Every pair damages big_cycle on both sides -> all bad -> full K^2 enumeration

N = 8K+2, M = 10K+2
"""

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
K = int(cmdlinearg('k'))

N = 8 * K + 2
M = 10 * K + 2

# Big cycle nodes: B[0..2K-1], with B[K] = room 3 (node 3)
B = [0] * (2 * K)
nxt = 4
for i in range(2 * K):
    if i == K:
        B[i] = 3
    else:
        B[i] = nxt
        nxt += 1

# Triangle nodes: a[t], b[t], c[t] for t = 0..2K-1
a = [0] * (2 * K)
b = [0] * (2 * K)
c = [0] * (2 * K)
for t in range(2 * K):
    a[t] = nxt; nxt += 1
    b[t] = nxt; nxt += 1
    c[t] = nxt; nxt += 1

assert nxt == N + 1

# Build edges (0-indexed array; edge ID = index + 1)
edges = []
partner = [0] * M  # 0-indexed partner mapping

# === Big cycle: 2K edges (indices 0..2K-1) ===
# Lside (top half, on path1&path2): B[0]->B[1], ..., B[K-1]->B[K]  (indices 0..K-1)
# Rside (bottom half):              B[K]->B[K+1], ..., B[2K-1]->B[0] (indices K..2K-1)
for i in range(2 * K):
    edges.append((B[i], B[(i + 1) % (2 * K)]))

# === Triangle edges: 6K edges (indices 2K..8K-1) ===
# Triangle t: 3 edges at indices 2K+3t, 2K+3t+1, 2K+3t+2
#   lside1: (a[t], b[t])
#   lside2: (b[t], c[t])
#   rside:  (a[t], c[t])
for t in range(2 * K):
    edges.append((a[t], b[t]))
    edges.append((b[t], c[t]))
    edges.append((a[t], c[t]))

# === Bridges: 2K+2 edges (indices 8K..10K+1) ===
edges.append((1, B[0]))              # room1 -> B[0]
edges.append((B[K], a[0]))           # B[K]=room3 -> first triangle entry
for t in range(2 * K - 1):
    edges.append((c[t], a[t + 1]))   # between consecutive triangles
edges.append((c[2 * K - 1], 2))     # last triangle -> room2

assert len(edges) == M

# === Edge pairings ===

# 1. Big lside[i] <-> tri[i] rside (i=0..K-1)
#    Makes lside sig contain (big_cycle, 1) and (tri_i, 0)
for i in range(K):
    e1 = i                        # big cycle lside
    e2 = 2 * K + 3 * i + 2       # tri i rside
    partner[e1] = e2
    partner[e2] = e1

# 2. Big rside[j] <-> tri[K+j] lside1 (j=0..K-1)
#    Makes rside sig contain (big_cycle, 0) and (tri_{K+j}, 1)
for j in range(K):
    e1 = K + j                    # big cycle rside
    e2 = 2 * K + 3 * (K + j)     # tri K+j lside1
    partner[e1] = e2
    partner[e2] = e1

# 3. Tri[0..K-1]: pair lside1 <-> lside2 (internal to triangle)
for i in range(K):
    e1 = 2 * K + 3 * i           # tri i lside1
    e2 = 2 * K + 3 * i + 1       # tri i lside2
    partner[e1] = e2
    partner[e2] = e1

# 4. Tri[K..2K-1]: pair lside2 <-> rside (internal to triangle)
for j in range(K):
    e1 = 2 * K + 3 * (K + j) + 1  # tri K+j lside2
    e2 = 2 * K + 3 * (K + j) + 2  # tri K+j rside
    partner[e1] = e2
    partner[e2] = e1

# 5. Bridges: pair consecutive
for i in range(0, 2 * K + 2, 2):
    e1 = 8 * K + i
    e2 = 8 * K + i + 1
    partner[e1] = e2
    partner[e2] = e1

# Verify pairings
for i in range(M):
    assert partner[i] != i, f"Edge {i+1} paired with itself"
    assert partner[partner[i]] == i, f"Pairing not symmetric at edge {i+1}"

# Output (1-indexed)
print(N, M)
for i in range(M):
    u, v = edges[i]
    print(u, v, partner[i] + 1)
