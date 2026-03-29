#!/usr/bin/env python3

import random
import sys
from collections import deque


def cmdlinearg(name, default=None):
    for arg in sys.argv[1:]:
        if arg.startswith(name + "="):
            return arg.split("=", 1)[1]
    if default is None:
        raise SystemExit(f"missing parameter {name}")
    return default


def add_edge(edges, u, v):
    edges.append((u, v))
    return len(edges)


def add_path(edges, next_vertex, start, end, length):
    edge_ids = []
    cur = start
    for _ in range(length - 1):
        new_vertex = next_vertex
        next_vertex += 1
        edge_ids.append(add_edge(edges, cur, new_vertex))
        cur = new_vertex
    edge_ids.append(add_edge(edges, cur, end))
    return edge_ids, next_vertex


def add_two_path_cycle(edges, next_vertex, start, end, left_len, right_len):
    left_edges, next_vertex = add_path(edges, next_vertex, start, end, left_len)
    right_edges, next_vertex = add_path(edges, next_vertex, start, end, right_len)
    return left_edges, right_edges, next_vertex


def build_adj(n, edges):
    adj = [[] for _ in range(n + 1)]
    for edge_id, (u, v) in enumerate(edges, start=1):
        adj[u].append((v, edge_id))
        adj[v].append((u, edge_id))
    return adj


def bfs_path(adj, start, goal):
    parent = [-1] * len(adj)
    parent_edge = [-1] * len(adj)
    q = deque([start])
    parent[start] = start
    while q:
        u = q.popleft()
        if u == goal:
            break
        for v, edge_id in adj[u]:
            if parent[v] == -1:
                parent[v] = u
                parent_edge[v] = edge_id
                q.append(v)
    path_edges = []
    cur = goal
    while cur != start:
        path_edges.append(parent_edge[cur])
        cur = parent[cur]
    path_edges.reverse()
    return path_edges


def bfs_dist(adj, start):
    dist = [-1] * len(adj)
    parent = [-1] * len(adj)
    order = []
    q = deque([start])
    dist[start] = 0
    parent[start] = start
    while q:
        u = q.popleft()
        order.append(u)
        for v, _ in adj[u]:
            if dist[v] == -1:
                dist[v] = dist[u] + 1
                parent[v] = u
                q.append(v)
    return dist, parent, order


def choose_cycle_lengths(n, cycles, style, rng):
    lengths = []
    remaining_vertices = n - 1
    remaining_cycles = cycles

    for _ in range(cycles):
        max_use = remaining_vertices - 2 * (remaining_cycles - 1)
        if style == "small":
            use = 2
        elif style == "large":
            low = max(2, max_use - min(24, max_use - 2))
            use = rng.randint(low, max_use)
        else:
            use = rng.randint(2, max_use)
        lengths.append(use + 1)
        remaining_vertices -= use
        remaining_cycles -= 1

    return lengths, remaining_vertices


def build_special_line(n):
    if n < 3:
        raise SystemExit("line generator requires n >= 3")

    order = [1]
    if n >= 4:
        order.extend(range(4, n + 1))
    order.extend([3, 2])

    if len(order) != n:
        raise SystemExit("line generator built the wrong number of vertices")

    edges = []
    for idx in range(len(order) - 1):
        edges.append((order[idx], order[idx + 1]))
    return edges


def build_special_ops_cap(common_left, common_right):
    if common_left < 2 or common_right < 2:
        raise SystemExit("ops_cap generator needs at least two edges on each side")

    edges = []
    next_vertex = 4

    mid1 = next_vertex
    next_vertex += 1
    mid2 = next_vertex
    next_vertex += 1

    a_off_len = max(common_left, common_right + 1)
    a_on_len = common_right
    a_off, a_on, next_vertex = add_two_path_cycle(edges, next_vertex, 1, mid1, a_off_len, a_on_len)

    _, b_off, next_vertex = add_two_path_cycle(edges, next_vertex, mid1, mid2, 1, 2)
    _, c_off, next_vertex = add_two_path_cycle(edges, next_vertex, mid2, 2, 1, 2)

    neutral_mid = next_vertex
    next_vertex += 1
    neutral_leaf = next_vertex
    next_vertex += 1
    add_edge(edges, mid2, neutral_mid)
    add_edge(edges, neutral_mid, neutral_leaf)

    left_len = common_left + 1
    right_len = max(common_right, left_len + 1)
    _, _, next_vertex = add_two_path_cycle(edges, next_vertex, 1, 3, left_len, right_len)

    n = next_vertex - 1
    
    return n, edges


def build_graph(n, m, structure, cycle_style, vertex_disjoint, rng):
    cycles = m - (n - 1)
    if cycles < 0 or 2 * cycles > n - 1:
        raise SystemExit("requested n,m do not form a connected cactus graph")

    if vertex_disjoint:
        if 3 * cycles > n:
            raise SystemExit("requested n,m do not form a connected cactus graph with vertex-disjoint cycles")
        return build_graph_vertex_disjoint(n, m, structure, cycle_style, rng)

    cycle_lengths, tree_edges = choose_cycle_lengths(n, cycles, cycle_style, rng)
    blocks = [("cycle", length) for length in cycle_lengths] + [("edge", None)] * tree_edges

    if structure == "path":
        blocks.sort(key=lambda item: 0 if item[0] == "edge" else 1)
    else:
        rng.shuffle(blocks)

    edges = []
    vertices = [1]
    cycle_count = [0] * (n + 1)
    last_vertex = 1
    next_vertex = 2

    def pick_attachment(want_cycle):
        nonlocal last_vertex
        candidates = vertices
        if want_cycle and vertex_disjoint:
            candidates = [v for v in candidates if cycle_count[v] == 0]
            if not candidates:
                raise SystemExit("no valid attachment for vertex-disjoint cycle")

        if structure == "hub":
            if (not want_cycle or not vertex_disjoint or cycle_count[1] == 0) and rng.random() < 0.65:
                return 1
            return rng.choice(candidates)
        if structure == "deep":
            recent = candidates[max(0, len(candidates) - max(8, len(candidates) // 3)) :]
            return rng.choice(recent)
        if structure == "path":
            if (not want_cycle or not vertex_disjoint or cycle_count[last_vertex] == 0) and rng.random() < 0.8:
                return last_vertex
            return rng.choice(candidates)
        return rng.choice(candidates)

    for kind, payload in blocks:
        if kind == "edge":
            attach = pick_attachment(False)
            new_vertex = next_vertex
            next_vertex += 1
            vertices.append(new_vertex)
            edges.append((attach, new_vertex))
            last_vertex = new_vertex
            continue

        cycle_len = payload
        attach = pick_attachment(True)
        first_new = next_vertex
        prev = attach
        for _ in range(cycle_len - 1):
            new_vertex = next_vertex
            next_vertex += 1
            vertices.append(new_vertex)
            edges.append((prev, new_vertex))
            prev = new_vertex
        edges.append((prev, attach))
        cycle_count[attach] += 1
        for vertex in range(first_new, next_vertex):
            cycle_count[vertex] = 1
        last_vertex = prev

    if next_vertex != n + 1 or len(edges) != m:
        raise SystemExit("generator built the wrong graph size")

    return edges


def build_graph_vertex_disjoint(n, m, structure, cycle_style, rng):
    cycles = m - (n - 1)
    cycle_lengths, _ = choose_cycle_lengths(n - cycles + 1, cycles, cycle_style, rng)

    edges = []
    existing_vertices = [1]
    next_vertex = 2
    connector = 1

    def pick_cycle_exit(cycle_vertices):
        if structure == "path":
            return cycle_vertices[-1]
        if structure == "hub":
            return cycle_vertices[0]
        if structure == "deep":
            return cycle_vertices[-1]
        return rng.choice(cycle_vertices)

    for cycle_index, cycle_len in enumerate(cycle_lengths):
        cycle_vertices = [connector]
        prev = connector
        for _ in range(cycle_len - 1):
            new_vertex = next_vertex
            next_vertex += 1
            existing_vertices.append(new_vertex)
            cycle_vertices.append(new_vertex)
            edges.append((prev, new_vertex))
            prev = new_vertex
        edges.append((prev, connector))

        if cycle_index + 1 < cycles:
            bridge_from = pick_cycle_exit(cycle_vertices)
            connector = next_vertex
            next_vertex += 1
            existing_vertices.append(connector)
            edges.append((bridge_from, connector))

    while next_vertex <= n:
        if structure == "path":
            attach = existing_vertices[-1]
        elif structure == "deep":
            attach = existing_vertices[max(0, len(existing_vertices) - 1 - rng.randint(0, min(10, len(existing_vertices) - 1)))]
        elif structure == "hub" and rng.random() < 0.5:
            attach = 1
        else:
            attach = rng.choice(existing_vertices)
        new_vertex = next_vertex
        next_vertex += 1
        existing_vertices.append(new_vertex)
        edges.append((attach, new_vertex))

    if len(edges) != m:
        raise SystemExit("generator built the wrong graph size")
    return edges


def pick_terminals(adj):
    root = 1
    dist, parent, bfs_order = bfs_dist(adj, root)

    branch = [0] * len(adj)
    for vertex in bfs_order[1:]:
        if parent[vertex] == root:
            branch[vertex] = vertex
        else:
            branch[vertex] = branch[parent[vertex]]

    best = []
    for vertex in range(2, len(adj)):
        best.append((dist[vertex], vertex))
    best.sort(reverse=True)

    v2 = best[0][1]
    branch2 = branch[v2]

    v3 = None
    for _, vertex in best[1:]:
        if branch[vertex] != branch2:
            v3 = vertex
            break
    if v3 is None:
        v3 = best[1][1]

    return root, v2, v3


def assign_dependencies(m, dep_mode, p12_edges, p13_edges, rng):
    """
    Fredriks pizzeria requires symmetric edge pairing:
    Every edge is paired with exactly one other edge (bijection/perfect matching).
    M is guaranteed to be even when this is called.
    """
    ids = list(range(1, m + 1))
    o = [0] * (m + 1)
    unpaired = set(ids)

    def pair_up(a, b):
        o[a] = b
        o[b] = a
        unpaired.remove(a)
        unpaired.remove(b)

    def pair_focus(focus_list):
        rng.shuffle(focus_list)
        for i in range(0, len(focus_list) - 1, 2):
            a, b = focus_list[i], focus_list[i+1]
            if a in unpaired and b in unpaired:
                pair_up(a, b)

    if dep_mode in ("path12", "mixed"):
        pair_focus(p12_edges[:])
        
    if dep_mode in ("path13", "mixed"):
        pair_focus(p13_edges[:])
        
    if dep_mode == "cross":
        # Pair 1->2 edges specifically with 1->3 edges
        rng.shuffle(p12_edges)
        rng.shuffle(p13_edges)
        limit = min(len(p12_edges), len(p13_edges))
        for i in range(limit):
            a, b = p12_edges[i], p13_edges[i]
            if a in unpaired and b in unpaired and a != b:
                pair_up(a, b)

    # Fill remaining edges to form a perfect matching
    rest = list(unpaired)
    rng.shuffle(rest)
    for i in range(0, len(rest), 2):
        pair_up(rest[i], rest[i+1])

    return o


def ensure_even_m(n, edges):
    """Appends an isolated dummy edge attached to room 2 if M is odd."""
    if len(edges) % 2 != 0:
        n += 1
        edges.append((2, n))
    return n, edges


def main():
    seed = int(cmdlinearg("seed", "0"))
    rng = random.Random(seed)

    special = cmdlinearg("special", "")
    n = int(cmdlinearg("n", "0"))
    m = int(cmdlinearg("m", "0"))
    
    # Ensure M is even requested by the logic or auto-adjust
    if special == "" and m % 2 != 0:
        m += 1
        
    structure = cmdlinearg("structure", "random")
    cycle_style = cmdlinearg("cycle_style", "mixed")
    dep_mode = cmdlinearg("dep", "random")
    vertex_disjoint = int(cmdlinearg("vertex_disjoint", "0")) == 1

    if special == "line":
        edges = build_special_line(n)
        n, edges = ensure_even_m(n, edges)
        m = len(edges)
        
        adj = build_adj(n, edges)
        p12_edges = bfs_path(adj, 1, 2)
        p13_edges = bfs_path(adj, 1, 3)
        dependencies = assign_dependencies(m, dep_mode, p12_edges, p13_edges, rng)
        
        out = [f"{n} {m}\n"]
        for edge_id, (u, v) in enumerate(edges, start=1):
            a = min(u, v)
            b = max(u, v)
            out.append(f"{a} {b} {dependencies[edge_id]}\n")
        sys.stdout.write("".join(out))
        return

    if special == "ops_cap":
        common_left = int(cmdlinearg("common_left"))
        common_right = int(cmdlinearg("common_right"))
        n, edges = build_special_ops_cap(common_left, common_right)
        n, edges = ensure_even_m(n, edges)
        m = len(edges)
        
        adj = build_adj(n, edges)
        p12_edges = bfs_path(adj, 1, 2)
        p13_edges = bfs_path(adj, 1, 3)
        dependencies = assign_dependencies(m, dep_mode, p12_edges, p13_edges, rng)
        
        out = [f"{n} {m}\n"]
        for edge_id, (u, v) in enumerate(edges, start=1):
            a = min(u, v)
            b = max(u, v)
            out.append(f"{a} {b} {dependencies[edge_id]}\n")
        sys.stdout.write("".join(out))
        return

    edges = build_graph(n, m, structure, cycle_style, vertex_disjoint, rng)

    adj = build_adj(n, edges)

    old_root, old_two, old_three = pick_terminals(adj)
    p12_edges = bfs_path(adj, old_root, old_two)
    p13_edges = bfs_path(adj, old_root, old_three)

    relabel = [0] * (n + 1)
    relabel[old_root] = 1
    relabel[old_two] = 2
    relabel[old_three] = 3
    next_label = 4
    for vertex in range(1, n + 1):
        if relabel[vertex] == 0:
            relabel[vertex] = next_label
            next_label += 1

    dependencies = assign_dependencies(m, dep_mode, p12_edges, p13_edges, rng)

    out = [f"{n} {m}\n"]
    for edge_id, (u, v) in enumerate(edges, start=1):
        a = relabel[u]
        b = relabel[v]
        if a > b:
            a, b = b, a
        out.append(f"{a} {b} {dependencies[edge_id]}\n")
    sys.stdout.write("".join(out))


if __name__ == "__main__":
    main()