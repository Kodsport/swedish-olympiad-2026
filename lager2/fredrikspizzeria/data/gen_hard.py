#!/usr/bin/python3
"""
Generator for challenging test cases.

Modes:
- conflict_chain: Chain where each cycle on path 1->3 has edges paired with different cycles on path 1->2
- all_same_cycle: All edges on path 1->3 paired with edges from the same cycle on path 1->2
- interleaved_cycles: Complex interleaving of cycle pairings
- max_cycles: Maximum number of small cycles
- dense_cactus: Dense edge cactus structure
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


class GraphBuilder:
    def __init__(self):
        self.n = 3  # Reserve 1, 2, 3
        self.edges = []
        self.pairings = []
    
    def new_node(self):
        self.n += 1
        return self.n
    
    def add_edge(self, u, v):
        """Add edge and return its index."""
        idx = len(self.edges)
        self.edges.append((u, v))
        return idx
    
    def pair_edges(self, i, j):
        """Mark edges i and j as paired."""
        while len(self.pairings) <= max(i, j):
            self.pairings.append(-1)
        self.pairings[i] = j
        self.pairings[j] = i
    
    def finalize(self):
        """Ensure even number of edges by adding dummy edges if needed."""
        # Extend pairings array to match edges
        while len(self.pairings) < len(self.edges):
            self.pairings.append(-1)
        
        unpaired = [i for i in range(len(self.edges)) if self.pairings[i] == -1]
        
        # If odd number of unpaired, add a dummy edge and pair it
        while len(unpaired) % 2 == 1:
            a = self.new_node()
            e = self.add_edge(1, a)
            self.pairings.append(-1)
            unpaired.append(e)
        
        # Pair remaining unpaired edges
        random.shuffle(unpaired)
        for i in range(0, len(unpaired), 2):
            if i + 1 < len(unpaired):
                self.pair_edges(unpaired[i], unpaired[i + 1])
        
        return self.n, self.edges, self.pairings
    
    def output(self):
        n, edges, pairings = self.finalize()
        m = len(edges)
        print(n, m)
        for i in range(m):
            u, v = edges[i]
            print(u, v, pairings[i] + 1)


if mode == 'conflict_chain':
    # Create a scenario where path 1->2 goes through a chain of cycles,
    # and path 1->3 also goes through cycles where each edge is paired
    # with edges from DIFFERENT cycles on path 1->2.
    # This tests proper handling of multi-cycle conflicts.
    
    num_cycles_12 = int(cmdlinearg('c12', '5'))
    num_cycles_13 = int(cmdlinearg('c13', '3'))
    
    g = GraphBuilder()
    
    # Build path 1 -> ... -> 2 with cycles
    # Each cycle is: prev - A - B - prev (triangle missing one edge)
    # Actually, let's do: prev - A - B - C - prev (4-cycle)
    
    prev = 1
    cycles_12_edges = []  # [(left_edges, right_edges), ...]
    
    for _ in range(num_cycles_12):
        a = g.new_node()
        b = g.new_node()
        
        e_entry = g.add_edge(prev, a)
        e_left = g.add_edge(a, b)  # On path
        e_right = g.add_edge(prev, b)  # Alternative
        
        cycles_12_edges.append(([e_left], [e_right]))
        prev = b
    
    e_to_2 = g.add_edge(prev, 2)
    
    # Build path 1 -> ... -> 3 with cycles
    prev = 1
    cycles_13_edges = []
    
    for _ in range(num_cycles_13):
        a = g.new_node()
        b = g.new_node()
        
        e_entry = g.add_edge(prev, a)
        e_left = g.add_edge(a, b)
        e_right = g.add_edge(prev, b)
        
        cycles_13_edges.append(([e_left], [e_right]))
        prev = b
    
    e_to_3 = g.add_edge(prev, 3)
    
    # Pair edges strategically:
    # - Pair the entry edges with corresponding cycle edges
    # - Pair left side of cycle 13[i] with right side of cycle 12[i % c12]
    
    # First, collect unpaired edges and pair them randomly
    all_edges = list(range(len(g.edges)))
    random.shuffle(all_edges)
    
    paired = [False] * len(g.edges)
    
    # Strategic pairings: pair 13-left with 12-right
    for i in range(min(num_cycles_13, num_cycles_12)):
        e13_left = cycles_13_edges[i][0][0]
        e12_right = cycles_12_edges[i][1][0]
        g.pair_edges(e13_left, e12_right)
        paired[e13_left] = True
        paired[e12_right] = True
    
    # Pair remaining edges randomly
    unpaired = [e for e in all_edges if not paired[e]]
    random.shuffle(unpaired)
    for i in range(0, len(unpaired) - 1, 2):
        g.pair_edges(unpaired[i], unpaired[i + 1])
    
    g.output()

elif mode == 'split_early':
    # Path 1->2 and 1->3 split early, with complex interactions
    
    n_path_to_split = int(cmdlinearg('split', '3'))
    n_path_to_2 = int(cmdlinearg('to2', '5'))
    n_path_to_3 = int(cmdlinearg('to3', '5'))
    
    g = GraphBuilder()
    
    # Common path from 1 to split point
    prev = 1
    for _ in range(n_path_to_split):
        a = g.new_node()
        g.add_edge(prev, a)
        prev = a
    
    split = prev
    
    # Path from split to 2
    prev = split
    for _ in range(n_path_to_2):
        a = g.new_node()
        g.add_edge(prev, a)
        prev = a
    g.add_edge(prev, 2)
    
    # Path from split to 3
    prev = split
    for _ in range(n_path_to_3):
        a = g.new_node()
        g.add_edge(prev, a)
        prev = a
    g.add_edge(prev, 3)
    
    # Random pairings
    m = len(g.edges)
    order = list(range(m))
    random.shuffle(order)
    for i in range(0, m - 1, 2):
        g.pair_edges(order[i], order[i + 1])
    
    g.output()

elif mode == 'forced_no':
    # Create a graph where the answer must be "Nej"
    # Strategy: every edge on path 1->3 is paired with an edge on path 1->2
    
    path_len = int(cmdlinearg('len', '10'))
    
    g = GraphBuilder()
    
    # Path 1 -> 4 -> 5 -> ... -> 2
    prev = 1
    edges_to_2 = []
    for i in range(path_len):
        a = g.new_node()
        e = g.add_edge(prev, a)
        edges_to_2.append(e)
        prev = a
    e = g.add_edge(prev, 2)
    edges_to_2.append(e)
    
    # Path 1 -> ... -> 3 (same length)
    prev = 1
    edges_to_3 = []
    for i in range(path_len):
        a = g.new_node()
        e = g.add_edge(prev, a)
        edges_to_3.append(e)
        prev = a
    e = g.add_edge(prev, 3)
    edges_to_3.append(e)
    
    # Pair edges: each edge to 3 paired with corresponding edge to 2
    for i in range(len(edges_to_3)):
        g.pair_edges(edges_to_3[i], edges_to_2[i])
    
    g.output()

elif mode == 'forced_yes':
    # Create a graph where the answer must be "Ja"
    # Strategy: there's an edge on path 1->3 whose partner is NOT on path 1->2
    # Keep graph connected by attaching dummy edges to existing nodes
    
    path_len = int(cmdlinearg('len', '10'))
    
    g = GraphBuilder()
    
    # Path 1 -> ... -> 2
    prev = 1
    nodes_to_2 = [1]
    for i in range(path_len):
        a = g.new_node()
        g.add_edge(prev, a)
        nodes_to_2.append(a)
        prev = a
    g.add_edge(prev, 2)
    
    # Path 1 -> ... -> 3 with edges paired to non-path edges
    prev = 1
    edges_to_3 = []
    nodes_to_3 = [1]
    for i in range(path_len):
        a = g.new_node()
        e = g.add_edge(prev, a)
        edges_to_3.append(e)
        nodes_to_3.append(a)
        prev = a
    e = g.add_edge(prev, 3)
    edges_to_3.append(e)
    
    # Add "dummy" edges that are connected to existing nodes (not on the paths)
    # Attach them to nodes on path to 2 (which isn't the critical path)
    for i, e in enumerate(edges_to_3):
        # Add edge from a node on path-to-2 to a new leaf
        attach_node = nodes_to_2[i % len(nodes_to_2)]
        leaf = g.new_node()
        dummy = g.add_edge(attach_node, leaf)
        g.pair_edges(e, dummy)
    
    g.output()

elif mode == 'cycle_conflict':
    # Create a cycle on path 1->2 and a cycle on path 1->3
    # Pair edges across cycles to create conflicts
    
    cycle_12_size = int(cmdlinearg('c12_size', '4'))
    cycle_13_size = int(cmdlinearg('c13_size', '4'))
    
    g = GraphBuilder()
    
    # Create cycle on path to 2
    # 1 -> a1 -> a2 -> ... -> ak -> 2
    #      |___________________|
    cycle_12_nodes = [1]
    for _ in range(cycle_12_size):
        cycle_12_nodes.append(g.new_node())
    
    cycle_12_left = []  # Edges on the "path" side
    cycle_12_right = []  # Edges on the "alternative" side
    
    for i in range(cycle_12_size):
        e = g.add_edge(cycle_12_nodes[i], cycle_12_nodes[i + 1])
        cycle_12_left.append(e)
    
    # Close the cycle
    for i in range(cycle_12_size - 1):
        e = g.add_edge(cycle_12_nodes[i], cycle_12_nodes[i + 2])
        cycle_12_right.append(e)
    
    # Connect to 2
    g.add_edge(cycle_12_nodes[-1], 2)
    
    # Create cycle on path to 3
    cycle_13_nodes = [1]
    for _ in range(cycle_13_size):
        cycle_13_nodes.append(g.new_node())
    
    cycle_13_left = []
    cycle_13_right = []
    
    for i in range(cycle_13_size):
        e = g.add_edge(cycle_13_nodes[i], cycle_13_nodes[i + 1])
        cycle_13_left.append(e)
    
    for i in range(cycle_13_size - 1):
        e = g.add_edge(cycle_13_nodes[i], cycle_13_nodes[i + 2])
        cycle_13_right.append(e)
    
    # Connect to 3
    g.add_edge(cycle_13_nodes[-1], 3)
    
    # Pair: 13-left with 12-right, and vice versa
    n_pairs = min(len(cycle_13_left), len(cycle_12_right))
    for i in range(n_pairs):
        g.pair_edges(cycle_13_left[i], cycle_12_right[i])
    
    n_pairs2 = min(len(cycle_13_right), len(cycle_12_left))
    for i in range(n_pairs2):
        g.pair_edges(cycle_13_right[i], cycle_12_left[i])
    
    # Pair remaining edges
    m = len(g.edges)
    unpaired = [i for i in range(m) if len(g.pairings) <= i or g.pairings[i] == -1]
    random.shuffle(unpaired)
    for i in range(0, len(unpaired) - 1, 2):
        g.pair_edges(unpaired[i], unpaired[i + 1])
    
    g.output()

elif mode == 'large_vertex_cactus':
    # Large vertex cactus for stress testing groups 4, 6
    
    n_target = int(cmdlinearg('n', '2000'))
    num_cycles = int(cmdlinearg('cycles', '100'))
    
    g = GraphBuilder()
    
    # Create a spine from 1 through cycles to a split point, then to 2 and 3
    nodes = [1]
    
    cycle_per_section = max(1, num_cycles // 3)
    
    # Section 1: cycles before split
    for _ in range(cycle_per_section):
        a = g.new_node()
        b = g.new_node()
        g.add_edge(nodes[-1], a)
        g.add_edge(a, b)
        g.add_edge(b, nodes[-1])
        nodes.append(a)
    
    split = nodes[-1]
    
    # Section 2: path to 2 with cycles
    branch_2 = [split]
    for _ in range(cycle_per_section):
        a = g.new_node()
        b = g.new_node()
        g.add_edge(branch_2[-1], a)
        g.add_edge(a, b)
        g.add_edge(b, branch_2[-1])
        branch_2.append(a)
    g.add_edge(branch_2[-1], 2)
    
    # Section 3: path to 3 with cycles
    branch_3 = [split]
    for _ in range(cycle_per_section):
        a = g.new_node()
        b = g.new_node()
        g.add_edge(branch_3[-1], a)
        g.add_edge(a, b)
        g.add_edge(b, branch_3[-1])
        branch_3.append(a)
    g.add_edge(branch_3[-1], 3)
    
    # Fill up to n_target with more edges
    while g.n < n_target - 10:
        target = random.choice(nodes + branch_2 + branch_3)
        if target in [1, 2, 3]:
            continue
        a = g.new_node()
        g.add_edge(target, a)
        nodes.append(a)
    
    # Random pairings
    m = len(g.edges)
    order = list(range(m))
    random.shuffle(order)
    for i in range(0, m - 1, 2):
        g.pair_edges(order[i], order[i + 1])
    
    g.output()

elif mode == 'many_small_cycles':
    # Many small cycles to stress cycle detection
    
    num_cycles = int(cmdlinearg('cycles', '1000'))
    
    g = GraphBuilder()
    
    prev = 1
    mid = None
    
    for i in range(num_cycles):
        a = g.new_node()
        b = g.new_node()
        
        g.add_edge(prev, a)
        g.add_edge(a, b)
        g.add_edge(b, a)  # Creates cycle a-b-a? No, need 3 nodes
        
        # Actually create triangle: prev - a - b - prev
        g.add_edge(b, prev)
        
        prev = a
        
        if i == num_cycles // 2:
            mid = prev
    
    g.add_edge(prev, 2)
    
    if mid:
        g.add_edge(mid, 3)
    else:
        g.add_edge(prev, 3)
    
    # Random pairings
    m = len(g.edges)
    order = list(range(m))
    random.shuffle(order)
    for i in range(0, m - 1, 2):
        g.pair_edges(order[i], order[i + 1])
    
    g.output()

else:
    print(f"Unknown mode: {mode}", file=sys.stderr)
    sys.exit(1)
