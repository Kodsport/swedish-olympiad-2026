#!/usr/bin/python3
"""
Generator for edge cases that test various incorrect solutions.

Modes:
- chain_cycles: Chain of cycles where path 1->2 and 1->3 share many cycles
- nested_pairs: Edges paired across different cycles in complex ways
- small_graph_no: Small graph where answer is definitively "Nej"
- small_graph_yes: Small graph where answer is definitively "Ja"
- max_constraints: Stress test at maximum constraints
- kill_bitset: More than 256 relevant cycle variables to break bitset solutions
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

def output_graph(n, edges, pairings):
    """Output the graph in required format."""
    m = len(edges)
    assert m % 2 == 0
    assert len(pairings) == m
    
    print(n, m)
    for i in range(m):
        u, v = edges[i]
        print(u, v, pairings[i] + 1)

def shuffle_graph(n, edges, pairings):
    """Shuffle node labels and edge order."""
    # Create node permutation (keeping 1, 2, 3 as special)
    perm = list(range(1, n + 1))
    fixed = [1, 2, 3]
    rest = [x for x in perm if x not in fixed]
    random.shuffle(rest)
    
    # Map old -> new
    mapping = {}
    mapping[1] = 1
    mapping[2] = 2
    mapping[3] = 3
    for i, x in enumerate(rest):
        mapping[x] = 4 + i
    
    # Reverse mapping for final assignment
    inv_mapping = {v: k for k, v in mapping.items()}
    final_perm = [0] * (n + 1)
    for old in range(1, n + 1):
        final_perm[old] = mapping[old]
    
    # Apply permutation to edges
    new_edges = [(final_perm[u], final_perm[v]) for u, v in edges]
    
    # Shuffle edge order
    m = len(edges)
    order = list(range(m))
    random.shuffle(order)
    
    shuffled_edges = [new_edges[order[i]] for i in range(m)]
    
    # Update pairings
    inv_order = [0] * m
    for i in range(m):
        inv_order[order[i]] = i
    
    new_pairings = [inv_order[pairings[order[i]]] for i in range(m)]
    
    return shuffled_edges, new_pairings


if mode == 'chain_cycles':
    # Create a chain of cycles where paths 1->2 and 1->3 share cycles
    # This tests solutions that don't properly track cycle dependencies
    
    num_cycles = int(cmdlinearg('cycles', '5'))
    cycle_size = int(cmdlinearg('cycle_size', '4'))
    
    edges = []
    n = 1  # Node counter
    
    # Start from node 1
    prev_node = 1
    n = 3  # Reserve 1, 2, 3
    
    # Create chain: 1 - cycle1 - cycle2 - ... - cycleK - branches to 2 and 3
    cycle_nodes = []
    for c in range(num_cycles):
        # Create a cycle
        cycle_start = n + 1
        for i in range(cycle_size):
            n += 1
        cycle_end = n
        cycle_nodes.append((cycle_start, cycle_end))
        
        # Connect to previous
        if c == 0:
            edges.append((1, cycle_start))
        else:
            edges.append((cycle_nodes[c-1][1], cycle_start))
        
        # Create cycle edges
        for i in range(cycle_size - 1):
            edges.append((cycle_start + i, cycle_start + i + 1))
        edges.append((cycle_end, cycle_start))  # Close cycle but remove last
    
    # Actually, let me redesign this more carefully
    edges = []
    n = 4  # Reserve 1, 2, 3, and start from 4
    
    # Simple chain: 1 -> A -> B -> ... -> split -> 2 and 3
    # Where each segment is a cycle
    
    # For simplicity, create: 1 connected to a big cycle, which then splits to 2 and 3
    # The key is to create complex pairings
    
    # Let's create: 1 - (cycle) - X - (cycle) - Y, where Y connects to both 2 and 3
    
    # Cycle 1: 1-4-5-6-1 (but we'll use edges differently)
    # Actually let me make it simpler
    
    # Create a path graph with some cycles
    # 1 -- 4 -- 5 -- 6 -- 7 -- 8 -- 9
    #      |         |         |
    #      +---------+         +-- 2
    #                          |
    #                          +-- 3
    
    edges = []
    n = 9
    
    # Main path
    edges.append((1, 4))  # 0
    edges.append((4, 5))  # 1
    edges.append((5, 6))  # 2
    edges.append((4, 6))  # 3 - creates cycle with 1,2
    edges.append((6, 7))  # 4
    edges.append((7, 8))  # 5
    edges.append((8, 9))  # 6
    edges.append((7, 9))  # 7 - creates cycle with 5,6
    edges.append((9, 2))  # 8
    edges.append((9, 3))  # 9
    
    # Pair edges: (0,9), (1,3), (2,4), (5,7), (6,8)
    pairings = [9, 3, 4, 1, 2, 7, 8, 5, 6, 0]
    
    m = len(edges)
    while m % 2 != 0:
        # Add dummy edges
        n += 1
        edges.append((9, n))
        edges.append((n, n))  # self loop? No, that's not allowed
        # Actually we need to be more careful
        break
    
    # Let's just ensure m is even
    if len(edges) % 2 != 0:
        n += 1
        edges.append((1, n))
        edges.append((n, 4))
        pairings = [9, 3, 4, 1, 2, 7, 8, 5, 6, 0, 11, 10]
    
    output_graph(n, edges, pairings)

elif mode == 'small_no':
    # Small graph where answer is "Nej"
    # Node 1 = Elsa, Node 2 = exit, Node 3 = rat
    # Make it so any cut that blocks rat also blocks Elsa
    
    n = 4
    edges = [
        (1, 2),  # 0
        (1, 3),  # 1
        (2, 4),  # 2
        (4, 3),  # 3
    ]
    # Pair edges: if we pair 0-1 and 2-3, then:
    # - Cutting 1 (1-3) also cuts 0 (1-2), blocking Elsa
    # - Cutting 3 (4-3) also cuts 2 (2-4), but path 1->2 uses edge 0
    # Wait, path 1->2 is just edge 0, path 1->3 is just edge 1
    # If we pair 0 with 1, cutting 1 kills both paths -> Nej
    
    pairings = [1, 0, 3, 2]
    output_graph(n, edges, pairings)

elif mode == 'small_yes':
    # Small graph where answer is "Ja"
    
    n = 5
    edges = [
        (1, 4),  # 0
        (4, 2),  # 1
        (1, 5),  # 2
        (5, 3),  # 3
    ]
    # Path 1->2: 0, 1
    # Path 1->3: 2, 3
    # If we pair 0-3 and 1-2:
    # Cutting 3 (5-3) also cuts 0 (1-4) -> blocks Elsa
    # Cutting 2 (1-5) also cuts 1 (4-2) -> blocks Elsa
    # Hmm, this is Nej
    
    # Let's pair 0-2 and 1-3:
    # Cutting 3 (5-3) also cuts 1 (4-2) -> blocks Elsa
    # Cutting 2 (1-5) also cuts 0 (1-4) -> blocks Elsa
    # Still Nej
    
    # Need different structure
    n = 6
    edges = [
        (1, 4),  # 0
        (4, 2),  # 1
        (1, 5),  # 2
        (5, 6),  # 3
        (6, 3),  # 4
        (5, 3),  # 5 - alternative path to 3
    ]
    # Path 1->2: 0, 1
    # Paths 1->3: (2, 3, 4) or (2, 5)
    # Pair: 0-4, 1-5, 2-3
    # Cutting 5 (5-3) cuts 1 (4-2) -> blocks Elsa path
    # Cutting 4 (6-3) cuts 0 (1-4) -> blocks Elsa path
    # Cutting 3 (5-6) cuts 2 (1-5) -> doesn't affect Elsa path!
    # So we can cut edge 3 and its pair 2, blocking rat without blocking Elsa
    
    pairings = [4, 5, 3, 2, 0, 1]
    output_graph(n, edges, pairings)

elif mode == 'kill_nicecactus':
    # Creates a case where joshua_nicecactus_bug1 fails
    # The bug is in handling multiple cycles
    
    # Create: path 1->2 goes through cycle A, path 1->3 goes through cycle B
    # Edges in B paired with edges in A in a way that the bug mishandles
    
    n = 9
    # Cycle A: 4-5-6-4 on path 1->2
    # Cycle B: 7-8-9-7 on path 1->3
    
    edges = [
        (1, 4),   # 0 - to cycle A
        (4, 5),   # 1 - cycle A
        (5, 6),   # 2 - cycle A
        (6, 4),   # 3 - cycle A (closing)
        (6, 2),   # 4 - from cycle A to exit
        (1, 7),   # 5 - to cycle B
        (7, 8),   # 6 - cycle B
        (8, 9),   # 7 - cycle B
        (9, 7),   # 8 - cycle B (closing)
        (9, 3),   # 9 - from cycle B to rat
    ]
    
    # Path 1->2: 0, 1 or 3, 2, 4 (through cycle A)
    # Path 1->3: 5, 6 or 8, 7, 9 (through cycle B)
    
    # Key pairing: pair edges from A-left with B-right and vice versa
    # A-left (on path): 1, 2
    # A-right: 3
    # B-left (on path): 6, 7  
    # B-right: 8
    
    # Pair: 0-9, 1-8, 2-5, 3-6, 4-7
    # Actually need even number of edges = 10, good
    
    # With this pairing:
    # To cut B-left (6 or 7), we cut A-right (3) or A-exit (4)
    # To cut B-right (8), we cut A-left (1)
    # 
    # Can we cut B without cutting A's path?
    # Cut 8 (B-right) -> cuts 1 (A-left on path) -> path still exists via 3,2
    # Cut 6 (B-left) -> cuts 3 (A-right) -> path still uses 1,2 -> OK!
    # So answer is Ja
    
    pairings = [9, 8, 5, 6, 7, 2, 3, 4, 1, 0]
    
    edges_s, pairings_s = shuffle_graph(n, edges, pairings)
    output_graph(n, edges_s, pairings_s)

elif mode == 'complex_cycle_chain':
    # Multiple cycles chained together, complex pairings
    num_cycles = int(cmdlinearg('cycles', '10'))
    
    edges = []
    n = 3  # Start after reserved nodes
    
    # Create a spine: 1 -> c1 -> c2 -> ... -> cK -> {2, 3}
    # Each cycle is a triangle (3 nodes, 3 edges)
    
    cycle_info = []  # [(start_node, [edge_indices])]
    prev_attach = 1
    
    for c in range(num_cycles):
        # Triangle with nodes n+1, n+2, n+3
        a, b, cc = n + 1, n + 2, n + 3
        n += 3
        
        base_edge = len(edges)
        edges.append((prev_attach, a))  # Connect from previous
        edges.append((a, b))
        edges.append((b, cc))
        edges.append((cc, a))  # Close triangle
        
        cycle_info.append((a, [base_edge + 1, base_edge + 2, base_edge + 3]))
        prev_attach = cc
    
    # Connect to 2 and 3
    edges.append((prev_attach, 2))
    edges.append((prev_attach, 3))
    
    m = len(edges)
    
    # Make m even if needed
    if m % 2 != 0:
        n += 1
        edges.append((prev_attach, n))
        m += 1
    
    # Create pairings: pair consecutive edges
    pairings = list(range(m))
    for i in range(0, m, 2):
        pairings[i] = i + 1
        pairings[i + 1] = i
    
    edges_s, pairings_s = shuffle_graph(n, edges, pairings)
    output_graph(n, edges_s, pairings_s)

elif mode == 'kill_bitset256':
    # Create a graph with > 256 cycles on the path 1->2
    # This will break solutions using 256-bit bitsets
    
    num_cycles = int(cmdlinearg('cycles', '300'))
    
    edges = []
    n = 3
    
    # Chain of triangles from 1 to 2, with 3 branching off near the end
    prev = 1
    
    for c in range(num_cycles):
        a, b = n + 1, n + 2
        n += 2
        
        # Triangle: prev - a - b - prev
        edges.append((prev, a))
        edges.append((a, b))
        edges.append((b, prev))
        
        prev = a
    
    # Connect to 2
    edges.append((prev, 2))
    
    # Branch off to 3 somewhere in the middle
    mid_cycle = num_cycles // 2
    mid_node = 4 + mid_cycle * 2  # Approximate middle node
    edges.append((mid_node, 3))
    
    m = len(edges)
    if m % 2 != 0:
        n += 1
        edges.append((2, n))
        m += 1
    
    # Random pairings
    pair_order = list(range(m))
    random.shuffle(pair_order)
    pairings = [0] * m
    for i in range(0, m, 2):
        pairings[pair_order[i]] = pair_order[i + 1]
        pairings[pair_order[i + 1]] = pair_order[i]
    
    output_graph(n, edges, pairings)

elif mode == 'same_cycle_pair':
    # Both edges of a pair are in the same cycle
    # Tests handling of this special case
    
    n = 6
    edges = [
        (1, 4),  # 0
        (4, 5),  # 1 - cycle
        (5, 6),  # 2 - cycle
        (6, 4),  # 3 - cycle, paired with 1
        (5, 2),  # 4
        (6, 3),  # 5
    ]
    
    # Cycle: 4-5-6-4 (edges 1, 2, 3)
    # Pair 1 and 3 (both in cycle)
    # Pair 0 and 4
    # Pair 2 and 5
    
    pairings = [4, 3, 5, 1, 0, 2]
    
    edges_s, pairings_s = shuffle_graph(n, edges, pairings)
    output_graph(n, edges_s, pairings_s)

elif mode == 'cross_path_pair':
    # Edges on path 1->2 paired with edges on path 1->3
    
    n = 7
    edges = [
        (1, 4),  # 0 - shared
        (4, 5),  # 1 - to 2
        (5, 2),  # 2 - to 2
        (4, 6),  # 3 - to 3
        (6, 7),  # 4 - to 3
        (7, 3),  # 5 - to 3
    ]
    
    # Pair edges on different paths
    pairings = [5, 4, 3, 2, 1, 0]
    
    edges_s, pairings_s = shuffle_graph(n, edges, pairings)
    output_graph(n, edges_s, pairings_s)

elif mode == 'vertex_cactus_stress':
    # Create a vertex cactus (each node in at most one cycle)
    # Stress test for group 4 and 6
    
    n_target = int(cmdlinearg('n', '2000'))
    
    edges = []
    n = 3
    
    # Create random vertex cactus by chaining triangles and single edges
    prev = 1
    
    while n < n_target - 10:
        choice = random.choice(['triangle', 'edge', 'edge'])
        
        if choice == 'triangle' and n + 2 <= n_target:
            a, b = n + 1, n + 2
            n += 2
            edges.append((prev, a))
            edges.append((a, b))
            edges.append((b, prev))
            prev = random.choice([a, b])
        else:
            a = n + 1
            n += 1
            edges.append((prev, a))
            prev = a
    
    # Connect to 2 and 3
    edges.append((prev, 2))
    
    # Find a good spot for 3
    if len(edges) > 4:
        # Get a node from early in the chain
        early_edge = edges[len(edges) // 4]
        attach_node = random.choice(early_edge)
        if attach_node in [2]:
            attach_node = prev
        edges.append((attach_node, 3))
    else:
        edges.append((prev, 3))
    
    m = len(edges)
    while m % 2 != 0:
        n += 1
        edges.append((prev, n))
        m += 1
    
    # Random pairings
    pair_order = list(range(m))
    random.shuffle(pair_order)
    pairings = [0] * m
    for i in range(0, m, 2):
        pairings[pair_order[i]] = pair_order[i + 1]
        pairings[pair_order[i + 1]] = pair_order[i]
    
    output_graph(n, edges, pairings)

else:
    print(f"Unknown mode: {mode}", file=sys.stderr)
    sys.exit(1)
