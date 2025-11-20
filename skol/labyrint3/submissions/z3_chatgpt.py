#!/usr/bin/env python3
# Requires z3: pip install z3-solver

from z3 import Ints, Solver, Int, sat
from collections import deque
import sys

def read_input():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    R = int(next(it)); C = int(next(it))
    grid = []
    for _ in range(R):
        row = next(it).strip()
        assert len(row) == C
        grid.append(row)
    queries = []
    for _ in range(5):
        r1 = int(next(it)); c1 = int(next(it)); r2 = int(next(it)); c2 = int(next(it))
        queries.append((r1, c1, r2, c2))
    return R, C, grid, queries

def build_graph(R, C, grid):
    nodes = []
    is_free = {}
    for i in range(R):
        for j in range(C):
            free = (grid[i][j] == '.')
            is_free[(i,j)] = free
            if free:
                nodes.append((i,j))
    # edges: from (i,j) -> (ni,nj) with translation (tx,ty)
    edges = {n: [] for n in nodes}
    dirs = [(-1,0),(1,0),(0,-1),(0,1)]
    for (i,j) in nodes:
        for dx,dy in dirs:
            raw_i = i + dx
            raw_j = j + dy
            ni = raw_i % R
            nj = raw_j % C
            if not is_free.get((ni,nj), False):
                continue
            tx = raw_i // R   # python // floors, works with negative
            ty = raw_j // C
            edges[(i,j)].append(((ni,nj),(tx,ty)))
    return nodes, edges, is_free

def solve_query(R,C,edges,is_free, q):
    (x1,y1,x2,y2) = q
    i1 = x1 % R; j1 = y1 % C; Q1 = x1 // R; P1 = y1 // C
    i2 = x2 % R; j2 = y2 % C; Q2 = x2 // R; P2 = y2 // C
    s = (i1,j1); t = (i2,j2)
    if not is_free.get(s, False) or not is_free.get(t, False):
        return False
    # BFS from s to get potentials (tile translations along chosen path)
    from collections import deque
    pot = {}  # node -> (px,py) translation from s to node
    dq = deque()
    pot[s] = (0,0)
    dq.append(s)
    while dq:
        u = dq.popleft()
        for v,(tx,ty) in edges.get(u,[]):
            if v not in pot:
                pu = pot[u]
                pot[v] = (pu[0] + tx, pu[1] + ty)
                dq.append(v)
    if t not in pot:
        return False
    # Collect cycle vectors from edges inside visited set
    cycles = []
    for u in pot:
        for v,(tx,ty) in edges.get(u,[]):
            if v in pot:
                # cycle = pot[u] + (tx,ty) - pot[v]
                cu = pot[u]
                cv = pot[v]
                cycle = (cu[0] + tx - cv[0], cu[1] + ty - cv[1])
                if cycle != (0,0):
                    cycles.append(cycle)
    # base translation from s to t (from our chosen paths) is pot[t]
    base = pot[t]  # (bx,by)
    delta_tiles = (Q2 - Q1, P2 - P1)
    residual = (delta_tiles[0] - base[0], delta_tiles[1] - base[1])
    # Check if residual is in integer span of cycles
    m = len(cycles)
    if m == 0:
        return residual == (0,0)
    # Use Z3: find integers k0..k_{m-1} such that sum k_i * cycles[i] == residual
    ks = [Int(f'k{i}') for i in range(m)]
    s = Solver()
    sum_x = None
    sum_y = None
    # build sums
    sx = None
    sy = None
    from z3 import Sum
    sx = Sum([ks[i] * cycles[i][0] for i in range(m)])
    sy = Sum([ks[i] * cycles[i][1] for i in range(m)])
    s.add(sx == residual[0])
    s.add(sy == residual[1])
    if s.check() == sat:
        return True
    else:
        return False

def main():
    R,C,grid,queries = read_input()
    nodes, edges, is_free = build_graph(R,C,grid)
    out = []
    for q in queries:
        ok = solve_query(R,C,edges,is_free,q)
        out.append("Ja\n" if ok else "Nej\n")
    sys.stdout.write(''.join(out))

if __name__ == "__main__":
    main()
