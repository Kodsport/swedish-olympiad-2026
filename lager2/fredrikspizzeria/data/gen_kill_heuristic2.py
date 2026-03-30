#!/usr/bin/python3
# This generator exploits the naive MAX_OPS = 2500 early break heuristic.
# It places 2500+ conflicting combinations up front, followed by the only 
# true non-conflicting answer at the end of the arrays.

import sys

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

# Read parameters
seed = cmdlinearg('seed', '0')
n_arg = int(cmdlinearg('n', '2000'))

# The number of edges per branch limits. N = 3K + 5.
k = (n_arg - 5) // 3
if k < 60:
    # Need at least k=60 to guarantee k^2 > 2500 ops. 
    # n=2000 allows us to easily use K ~ 665
    k = 60 

# Explicit Nodes alloc
W = 4
Z = 5
curr_node = 6

def get_nodes(count):
    global curr_node
    res = list(range(curr_node, curr_node + count))
    curr_node += count
    return res

L = get_nodes(k - 1)
R = get_nodes(k - 1)
X = get_nodes(k)
Y = get_nodes(k - 1)
D = get_nodes(3)

edges =[]
def add_edge(u, v):
    edges.append((u, v))
    return len(edges)  # 1-indexed

# By explicitly enforcing the generation order, we rigidly control the 
# competitor's DFS tree exploration priorities: L branch before R branch, etc.
E_12_bridge = add_edge(1, W)
E_13_bridge = add_edge(1, Z)

E_L =[]
curr = W
for i in range(k - 1):
    E_L.append(add_edge(curr, L[i]))
    curr = L[i]
E_L.append(add_edge(curr, 2))

E_R =[]
curr = W
for i in range(k - 1):
    E_R.append(add_edge(curr, R[i]))
    curr = R[i]
E_R.append(add_edge(curr, 2))

A =[]
curr = Z
for i in range(k):
    A.append(add_edge(curr, X[i]))
    curr = X[i]
A.append(add_edge(curr, 3))

B =[]
curr = Z
for i in range(k - 1):
    B.append(add_edge(curr, Y[i]))
    curr = Y[i]
B.append(add_edge(curr, 3))

D1 = add_edge(2, D[0])
D2 = add_edge(2, D[1])
D3 = add_edge(2, D[2])

pairings = {}
def pair(e1, e2):
    pairings[e1] = e2
    pairings[e2] = e1

# Bridge pair avoids the path13_bridge trivial bypass
pair(E_12_bridge, E_13_bridge)

# Make the first k * k operations aggressively conflict
for i in range(k):
    pair(E_L[i], A[i])
    pair(E_R[i], B[i])

# Our secret valid solution (placed out of reach due to MAX_OPS)
pair(A[k], D1)
# Keep graph uniform and m even parity
pair(D2, D3)

n = curr_node - 1
m = len(edges)
assert m % 2 == 0

print(f"{n} {m}")
for i, (u, v) in enumerate(edges):
    e_id = i + 1
    p_id = pairings[e_id]
    print(f"{u} {v} {p_id}")