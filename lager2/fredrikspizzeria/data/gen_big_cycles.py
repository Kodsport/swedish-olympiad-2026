#!/usr/bin/python3
"""
Generator for big cycle test cases.

Modes:
- one_cycle_yes: One big cycle, answer Ja. Rat edges paired with dummies.
- one_cycle_no: One big cycle, answer Nej. Rat edges paired with Elsa edges.
- three_cycles_yes: Three big cycles, answer Ja.
- three_cycles_no_cross: Three cycles, Nej via cross-pairing between cycles.
- three_cycles_no_direct: Three cycles, Nej via direct path pairing.
- three_cycles_no_shared: Three cycles, Nej via shared cycle forcing.
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
mode = cmdlinearg('mode')
target_n = int(cmdlinearg('n', '100000'))


edges = []
pairings = []
n = 3  # reserve 1, 2, 3


def add_edge(u, v):
    global n
    edges.append((u, v))
    pairings.append(-1)
    return len(edges) - 1


def pair_edges(e1, e2):
    pairings[e1] = e2
    pairings[e2] = e1


def new_node():
    global n
    n += 1
    return n


def add_path(start, end, length):
    """Build a path from start to end using length edges (length-1 internal nodes)."""
    edge_ids = []
    prev = start
    for i in range(length - 1):
        v = new_node()
        edge_ids.append(add_edge(prev, v))
        prev = v
    edge_ids.append(add_edge(prev, end))
    return edge_ids


def add_dummy_pair_for(edge_id):
    """Create a dummy leaf edge attached to node 2 and pair it with edge_id."""
    d = new_node()
    de = add_edge(2, d)
    pair_edges(edge_id, de)


def finalize_pairings():
    """Pair all remaining unpaired edges randomly."""
    unpaired = [i for i in range(len(edges)) if pairings[i] == -1]
    if len(unpaired) % 2 == 1:
        d = new_node()
        unpaired.append(add_edge(2, d))
    random.shuffle(unpaired)
    for i in range(0, len(unpaired), 2):
        pair_edges(unpaired[i], unpaired[i + 1])


def shuffle_and_output():
    """Shuffle node labels (keeping 1,2,3 fixed) and edge order, then output."""
    m = len(edges)
    assert m % 2 == 0, f"m={m} is odd"
    assert all(p != -1 for p in pairings), "unpaired edges remain"

    # Build node permutation
    perm = list(range(n + 1))  # identity
    rest = list(range(4, n + 1))
    random.shuffle(rest)
    for i, v in enumerate(rest):
        perm[v] = 4 + i

    # Apply to edges
    new_edges = [(perm[u], perm[v]) for u, v in edges]

    # Shuffle edge order
    order = list(range(m))
    random.shuffle(order)
    inv_order = [0] * m
    for i in range(m):
        inv_order[order[i]] = i

    shuffled_edges = [new_edges[order[i]] for i in range(m)]
    shuffled_pairings = [inv_order[pairings[order[i]]] for i in range(m)]

    print(n, m)
    for i in range(m):
        u, v = shuffled_edges[i]
        print(u, v, shuffled_pairings[i] + 1)


if mode == 'one_cycle_yes':
    # One big cycle through 1, 2, 3.
    # Left half: 1 -> ... -> 2 (Elsa's escape path)
    # Right half: 1 -> ... -> 3 -> ... -> 2
    # Answer YES: pair rat-side edges with dummies so we can cut them freely.
    
    # Place 3 on the right half of the cycle.
    # Left: 1 -> ... -> 2 with half_len edges
    # Right: 1 -> ... -> 3 -> ... -> 2
    half_len = target_n // 2
    rat_pos = half_len // 2  # where node 3 sits on right half

    # Left path: 1 -> ... -> 2
    left_edges = add_path(1, 2, half_len)

    # Right path: 1 -> ... -> 3 -> ... -> 2  
    right_part1 = add_path(1, 3, rat_pos)
    right_part2 = add_path(3, 2, half_len - rat_pos)

    # Pair all right-side edges (rat's path) with dummies
    for e in right_part1 + right_part2:
        add_dummy_pair_for(e)

    finalize_pairings()
    shuffle_and_output()

elif mode == 'one_cycle_no':
    # One big cycle through 1, 2, 3.
    # Answer NO: every edge on rat's side is paired with an edge on Elsa's side.
    # So cutting any rat-path edge also cuts an Elsa-path edge.
    
    half_len = target_n // 2
    rat_pos = half_len // 2

    # Left path (Elsa): 1 -> ... -> 2
    left_edges = add_path(1, 2, half_len)

    # Right path: 1 -> ... -> 3 -> ... -> 2
    right_part1 = add_path(1, 3, rat_pos)
    right_part2 = add_path(3, 2, half_len - rat_pos)
    right_edges = right_part1 + right_part2

    # Pair right edges 1-to-1 with left edges
    pair_count = min(len(right_edges), len(left_edges))
    for i in range(pair_count):
        pair_edges(right_edges[i], left_edges[i])

    finalize_pairings()
    shuffle_and_output()

elif mode == 'three_cycles_yes':
    # Three big cycles connected in a chain: C1 -- C2 -- C3
    # Node 1 at start of C1, node 2 at end of C1-C2 junction, node 3 at end of C3.
    # Answer YES: can cut C3 edges freely (paired with dummies).
    
    cycle_size = max(10, (target_n - 10) // 3)
    half = cycle_size // 2

    # Cycle 1: junction node J1
    j1 = new_node()
    c1_left = add_path(1, j1, half)
    c1_right = add_path(1, j1, half)

    # Cycle 2: from J1 to J2, node 2 on it
    j2 = new_node()
    c2_left = add_path(j1, 2, half // 2)
    c2_mid = add_path(2, j2, half - half // 2)
    c2_right = add_path(j1, j2, half)

    # Cycle 3: from J2 to node 3
    end3 = new_node()
    c3_left = add_path(j2, 3, half // 2)
    c3_mid = add_path(3, end3, half - half // 2)
    c3_right = add_path(j2, end3, half)

    # To make answer YES: pair all C3 edges with dummies
    for e in c3_left + c3_mid + c3_right:
        add_dummy_pair_for(e)

    finalize_pairings()
    shuffle_and_output()

elif mode == 'three_cycles_no_cross':
    # Three big cycles. Answer NO via cross-pairing: edges from the rat's
    # cycle are paired with edges from the Elsa's cycle, so cutting any
    # rat-reachable edge also opens a hole in Elsa's escape path.
    
    cycle_size = max(10, (target_n - 10) // 3)
    half = cycle_size // 2

    # Cycle 1 (shared): 1 -> J1  
    j1 = new_node()
    c1_left = add_path(1, j1, half)
    c1_right = add_path(1, j1, half)

    # Cycle 2 (Elsa path): J1 -> 2
    c2_left = add_path(j1, 2, half)
    c2_right = add_path(j1, 2, half)

    # Cycle 3 (rat path): J1 -> 3
    c3_left = add_path(j1, 3, half)
    c3_right = add_path(j1, 3, half)

    # Cross pair: c3 edges with c2 edges (rat ↔ Elsa)
    all_c3 = c3_left + c3_right
    all_c2 = c2_left + c2_right
    pair_count = min(len(all_c3), len(all_c2))
    for i in range(pair_count):
        pair_edges(all_c3[i], all_c2[i])

    finalize_pairings()
    shuffle_and_output()

elif mode == 'three_cycles_no_direct':
    # Three big cycles in a line: C1--C2--C3.
    # 1 at one end, 2 in the middle, 3 at the other end.
    # NO: rat's path edges all paired with Elsa's path edges.
    
    cycle_size = max(10, (target_n - 10) // 3)
    half = cycle_size // 2

    # Cycle 1: 1 → 2
    c1_left = add_path(1, 2, half)
    c1_right = add_path(1, 2, half)

    # Junction J between C2 and C3
    j = new_node()

    # Cycle 2: 2 → J
    c2_left = add_path(2, j, half)
    c2_right = add_path(2, j, half)

    # Cycle 3: J → 3
    c3_left = add_path(j, 3, half)
    c3_right = add_path(j, 3, half)

    # Pair: c2+c3 edges (on rat's path 1→3 via 2→J→3) with c1 edges (Elsa's cycle)
    rat_edges = c2_left + c2_right + c3_left + c3_right
    elsa_edges = c1_left + c1_right
    pair_count = min(len(rat_edges), len(elsa_edges))
    for i in range(pair_count):
        pair_edges(rat_edges[i], elsa_edges[i])

    finalize_pairings()
    shuffle_and_output()

elif mode == 'three_cycles_no_shared':
    # Three cycles, all sharing a common node. 1, 2, 3 each on a different cycle.
    # NO: every way to block the rat also blocks Elsa because the shared
    # cycle's edges are cross-paired.
    
    cycle_size = max(10, (target_n - 10) // 3)
    half = cycle_size // 2

    # Hub node
    hub = new_node()

    # Cycle A: hub -- 1
    ca_left = add_path(hub, 1, half)
    ca_right = add_path(hub, 1, half)

    # Cycle B: hub -- 2
    cb_left = add_path(hub, 2, half)
    cb_right = add_path(hub, 2, half)

    # Cycle C: hub -- 3
    cc_left = add_path(hub, 3, half)
    cc_right = add_path(hub, 3, half)

    # Cross-pair: C edges with B edges (cutting rat path kills Elsa path)
    all_cc = cc_left + cc_right
    all_cb = cb_left + cb_right
    pair_count = min(len(all_cc), len(all_cb))
    for i in range(pair_count):
        pair_edges(all_cc[i], all_cb[i])

    # Also cross-pair some A edges with remaining C/B edges to increase entanglement
    all_ca = ca_left + ca_right
    remaining_cc = [e for e in all_cc if pairings[e] == -1]
    pair_count2 = min(len(remaining_cc), len(all_ca))
    for i in range(pair_count2):
        pair_edges(remaining_cc[i], all_ca[i])

    finalize_pairings()
    shuffle_and_output()

else:
    print(f"Unknown mode: {mode}", file=sys.stderr)
    sys.exit(1)
