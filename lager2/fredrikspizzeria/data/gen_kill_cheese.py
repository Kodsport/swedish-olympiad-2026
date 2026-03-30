#!/usr/bin/python3
"""
Generator to kill joshua_cheese.cpp.

The bug: joshua_cheese doesn't filter forced_alive edges from rat-cycle rside/lside
when checking 2-SAT-like compatibility. So it counts rside edges whose linked partner
is a bridge on the player's path as usable, incorrectly outputting "Ja" when
the answer is "Nej".

Construction:
- Player path: 1 -> v1 -> v2 -> ... -> vk -> 2 (all bridges, forced_alive)
- Rat cycle: two routes from 1 to 3
  - Side A: 1 -> a1 -> ... -> ap -> 3  (in spanning tree, = rat's tree path)
  - Side B: 1 -> b1 -> ... -> bq -> 3  (non-tree, = cycle's rside)
- Linkages:
  - Side B edges <-> player-path bridge edges (closing side B kills player bridge)
  - Side A edges <-> each other (paired internally)
  - Remaining player bridges <-> each other (if any)
- Correct answer: "Nej" (can't block rat without killing player path)
- Cheese answer: "Ja" (counts side B edges in total_r, bad=0 < total_r)

Optionally adds random tree branches and extra paired edges to pad n/m.
"""

import sys
import random

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name, file=sys.stderr)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))

n_target = int(cmdlinearg('n'))
m_target = int(cmdlinearg('m', '0'))
mode = cmdlinearg('mode', 'basic')

# Core structure:
# p = side A internal nodes (side A has p+1 edges)
# q = side B internal nodes (side B has q+1 edges)  
# k = player path internal nodes (player path has k+1 edges)
# Constraints:
#   p+1 must be even (for internal pairing) => p is odd
#   q+1 <= k+1 (need enough player bridges) => q <= k
#   k-q must be even (remaining bridges pair internally) => k-q even
#   Total M = (k+1) + (p+1) + (q+1) = k+p+q+3 must be even => k+p+q odd
#   Total N = 3 + k + p + q

# We want M to be as close to m_target as possible, N close to n_target
# N = 3 + k + p + q => k + p + q = N - 3
# M = k + p + q + 3 = N => so M = N in the core structure

# If m_target > n_target, we need extra edges (more cycles)
# If m_target == 0, set M = N

if m_target == 0:
    m_target = n_target

# For the core, M = N (tree + 1 extra edge for the cycle)
# We need exactly one cycle. Extra edges for padding can be added as extra cycles.

def build_core(n_avail):
    """Build the core structure using up to n_avail nodes. Returns (n, edges, pairings)."""
    # Use n_avail - 3 nodes for internal structure
    # Split between p, q, k such that constraints are met
    budget = n_avail - 3  # nodes for k, p, q
    
    if budget < 3:
        # Minimum: p=1, q=1, k=1
        assert budget >= 3, f"Need at least 6 nodes, got {n_avail}"
    
    # Strategy: make q = k, p = budget - 2*q, p must be odd
    # q as large as possible to maximize the "fake" rside count
    # p >= 1 (need at least 2 side A edges for pairing)
    # p odd, k-q=0 (even), k+p+q must be odd => p+2q odd => p odd (since 2q even) ✓
    
    # p + 2q = budget, p >= 1, p odd, q >= 1
    # q = (budget - p) / 2
    # Maximize q => minimize p => p = 1
    
    p = 1
    if (budget - p) % 2 != 0:
        p = 3
    q = (budget - p) // 2
    k = q
    
    if q < 1:
        # Fallback for very small n
        p = 1
        q = 1
        k = 1
    
    assert p >= 1 and p % 2 == 1, f"p={p}"
    assert q >= 1, f"q={q}"
    assert k >= q, f"k={k}, q={q}"
    assert (k - q) % 2 == 0, f"k-q={k-q}"
    
    n = 3 + k + p + q
    m = k + p + q + 3
    assert m % 2 == 0, f"m={m}"
    
    # Build edges (1-indexed vertices)
    # Node assignment:
    #   1 = Elsa, 2 = exit, 3 = rat
    #   4..3+k = player path internal (v1..vk)
    #   3+k+1..3+k+p = side A internal (a1..ap)
    #   3+k+p+1..3+k+p+q = side B internal (b1..bq)
    
    edges = []  # list of (u, v)
    pairing = []  # pairing[i] = j means edge i linked with edge j (0-indexed)
    
    # Player path edges: 1 -> v1 -> v2 -> ... -> vk -> 2
    player_edges = []
    prev = 1
    for i in range(k):
        nxt = 4 + i
        edges.append((prev, nxt))
        player_edges.append(len(edges) - 1)
        prev = nxt
    edges.append((prev, 2))
    player_edges.append(len(edges) - 1)
    # player_edges has k+1 edge indices
    
    # Side A edges: 1 -> a1 -> a2 -> ... -> ap -> 3
    side_a_edges = []
    prev = 1
    for i in range(p):
        nxt = 3 + k + 1 + i
        edges.append((prev, nxt))
        side_a_edges.append(len(edges) - 1)
        prev = nxt
    edges.append((prev, 3))
    side_a_edges.append(len(edges) - 1)
    # side_a_edges has p+1 edge indices
    
    # Side B edges: 1 -> b1 -> b2 -> ... -> bq -> 3
    side_b_edges = []
    prev = 1
    for i in range(q):
        nxt = 3 + k + p + 1 + i
        edges.append((prev, nxt))
        side_b_edges.append(len(edges) - 1)
        prev = nxt
    edges.append((prev, 3))
    side_b_edges.append(len(edges) - 1)
    # side_b_edges has q+1 edge indices
    
    assert len(edges) == m
    
    # Create pairings
    pairing = [-1] * m
    
    # Side B[i] <-> player[i]  (q+1 pairs)
    for i in range(q + 1):
        a, b = side_b_edges[i], player_edges[i]
        pairing[a] = b
        pairing[b] = a
    
    # Side A edges paired internally (p+1 edges, p+1 is even)
    for i in range(0, p + 1, 2):
        a, b = side_a_edges[i], side_a_edges[i + 1]
        pairing[a] = b
        pairing[b] = a
    
    # Remaining player bridges paired internally
    remaining = player_edges[q + 1:]
    assert len(remaining) % 2 == 0, f"remaining={len(remaining)}"
    for i in range(0, len(remaining), 2):
        a, b = remaining[i], remaining[i + 1]
        pairing[a] = b
        pairing[b] = a
    
    assert all(p >= 0 for p in pairing), "Unpaired edge!"
    assert all(pairing[pairing[i]] == i for i in range(m)), "Pairing not symmetric!"
    assert all(pairing[i] != i for i in range(m)), "Self-paired edge!"
    
    return n, edges, pairing


def add_extra_tree_branches(n, edges, pairing, target_n, target_m):
    """Add random tree branches to pad to target_n/target_m.
    Extra edges come in linked pairs (bridges linked with each other)."""
    
    extra_n = target_n - n
    extra_m = target_m - len(edges)
    
    # Each extra pair of edges: adds 2 edges and 2 nodes (two branches from existing nodes)
    # Or: one branch adds 1 node and 1 edge; pair two branches together
    
    # We can add extra_m/2 pairs of edges, each pair adds 2 nodes
    # So extra_n should be >= extra_m (since each edge adds 1 node in a tree branch)
    # Actually: each extra edge is a bridge to a new leaf. We add them in pairs.
    
    if extra_m <= 0 and extra_n <= 0:
        return n, edges, pairing
    
    # First, add edges in pairs
    pairs_to_add = extra_m // 2
    
    current_n = n
    new_edges = list(edges)
    new_pairing = list(pairing)
    
    existing_nodes = list(range(1, n + 1))
    
    for _ in range(pairs_to_add):
        # Pick two random existing nodes to attach branches
        parent1 = random.choice(existing_nodes)
        parent2 = random.choice(existing_nodes)
        
        child1 = current_n + 1
        child2 = current_n + 2
        current_n += 2
        
        idx1 = len(new_edges)
        new_edges.append((parent1, child1))
        idx2 = len(new_edges)
        new_edges.append((parent2, child2))
        
        new_pairing.append(idx2)
        new_pairing.append(idx1)
        
        existing_nodes.append(child1)
        existing_nodes.append(child2)
    
    return current_n, new_edges, new_pairing


def shuffle_graph(n, edges, pairing):
    """Shuffle node labels (keeping 1,2,3 fixed) and edge order."""
    m = len(edges)
    
    # Permute non-special nodes
    other_nodes = list(range(4, n + 1))
    random.shuffle(other_nodes)
    node_map = {1: 1, 2: 2, 3: 3}
    for i, v in enumerate(other_nodes):
        node_map[4 + i] = v
    
    # Shuffle edge order
    order = list(range(m))
    random.shuffle(order)
    inv_order = [0] * m
    for i in range(m):
        inv_order[order[i]] = i
    
    new_edges = []
    new_pairing = [0] * m
    for i in range(m):
        old_idx = order[i]
        u, v = edges[old_idx]
        # Randomly swap u, v
        if random.random() < 0.5:
            u, v = v, u
        new_edges.append((node_map[u], node_map[v]))
        new_pairing[i] = inv_order[pairing[old_idx]]
    
    return new_edges, new_pairing


def output_graph(n, edges, pairing):
    m = len(edges)
    print(n, m)
    for i in range(m):
        u, v = edges[i]
        print(u, v, pairing[i] + 1)  # 1-indexed


# Build the graph
n_core, edges, pairing = build_core(n_target)

if m_target > len(edges):
    n_core, edges, pairing = add_extra_tree_branches(n_core, edges, pairing, n_target, m_target)

# Shuffle to avoid giving away the structure
edges, pairing = shuffle_graph(n_core, edges, pairing)

output_graph(n_core, edges, pairing)
