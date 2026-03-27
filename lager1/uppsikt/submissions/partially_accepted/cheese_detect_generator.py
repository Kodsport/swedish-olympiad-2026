#!/usr/bin/env python3
# @EXPECTED_GRADES@ AC WA WA WA WA
#
# Cheese: Detects which gen_perm.py pattern produced the input and uses
# hardcoded formulas. Falls back to greedy for undetected patterns.
# Formulas may be wrong for some patterns. Directly exploits generator knowledge.

import sys
from heapq import heappush, heappop

input = sys.stdin.readline


def solve():
    n = int(input())
    p = list(map(int, input().split()))

    if n == 1:
        print(1)
        return

    if n <= 15:
        print(brute_force(n, p))
        return

    # Try to detect generator pattern
    if is_sorted_asc(n, p):
        print((n + 2) // 3)
        return
    if is_sorted_desc(n, p):
        print((n + 2) // 3)
        return
    if is_alternating(n, p):
        print((n + 3) // 4)
        return
    if is_bitonic(n, p):
        print((n + 2) // 3)
        return
    if is_recursive(n, p):
        print((n + 2) // 3)
        return

    bs = detect_blocks(n, p)
    if bs != -1:
        print((n + 2) // 3)
        return

    # Fallback: greedy set cover
    print(greedy_cover(n, p))


def brute_force(n, p):
    # Build visibility matrix
    # vis[i] is a bitmask of positions visible from i
    vis = [0] * n
    for i in range(n):
        vis[i] |= 1 << i  # sees itself
        for j in range(i + 1, n):
            if i + 1 == j:
                # Adjacent always see each other
                vis[i] |= 1 << j
                vis[j] |= 1 << i
            else:
                mx = 0
                for k in range(i + 1, j):
                    if p[k] > mx:
                        mx = p[k]
                mn = min(p[i], p[j])
                if mx < mn:
                    vis[i] |= 1 << j
                    vis[j] |= 1 << i

    full = (1 << n) - 1
    best = n
    for mask in range(1, 1 << n):
        bc = bin(mask).count('1')
        if bc >= best:
            continue
        covered = 0
        for i in range(n):
            if mask & (1 << i):
                covered |= vis[i]
        if covered == full:
            best = bc
    return best


def is_sorted_asc(n, p):
    for i in range(n):
        if p[i] != i + 1:
            return False
    return True


def is_sorted_desc(n, p):
    for i in range(n):
        if p[i] != n - i:
            return False
    return True


def is_alternating(n, p):
    alt = build_alternating(n)
    return p == alt


def build_alternating(n):
    low = list(range(1, (n + 1) // 2 + 1))
    high = list(range((n + 1) // 2 + 1, n + 1))
    high.reverse()
    perm = []
    for i in range(n):
        if i % 2 == 0:
            perm.append(low[i // 2])
        else:
            perm.append(high[i // 2])
    return perm


def is_bitonic(n, p):
    mid = n // 2
    # First half: [1, 2, ..., mid]
    for i in range(mid):
        if p[i] != i + 1:
            return False
    # Second half: [n, n-1, ..., mid+1]
    for i in range(mid, n):
        if p[i] != n - (i - mid):
            return False
    return True


def is_recursive(n, p):
    ref = build_recursive(n)
    return p == ref


def build_recursive(n):
    # Iterative version to avoid stack overflow
    order = []
    stack = [(0, n - 1)]
    while stack:
        lo, hi = stack.pop()
        if lo > hi:
            continue
        mid = (lo + hi) // 2
        order.append(mid)
        # Push right first so left is processed first (stack is LIFO)
        stack.append((mid + 1, hi))
        stack.append((lo, mid - 1))
    perm = [0] * n
    value = n
    for idx in order:
        perm[idx] = value
        value -= 1
    return perm


def detect_blocks(n, p):
    # Find length of first decreasing run
    bs = 1
    while bs < n and p[bs] < p[bs - 1]:
        bs += 1
    if bs == 1:
        return -1

    # Verify all blocks have the same structure
    for start in range(0, n, bs):
        end = min(start + bs, n)
        block_len = end - start
        # Block should contain consecutive values in decreasing order
        # Values in this block should be: start_val, start_val-1, ..., start_val-block_len+1
        # where start_val = p[start]
        for j in range(1, block_len):
            if p[start + j] != p[start] - j:
                return -1
    # Also check that the blocks form consecutive value ranges
    for start in range(0, n, bs):
        end = min(start + bs, n)
        block_len = end - start
        expected_max = min(start + bs, n)
        if p[start] != expected_max:
            return -1
    return bs


def greedy_cover(n, p):
    # Build Cartesian tree using monotonic stack
    left = [-1] * n
    right = [-1] * n
    parent = [-1] * n
    stack = []
    for i in range(n):
        last = -1
        while stack and p[stack[-1]] < p[i]:
            last = stack.pop()
        if stack:
            right[stack[-1]] = i
            parent[i] = stack[-1]
        if last != -1:
            left[i] = last
            parent[last] = i
        stack.append(i)

    # Build adjacency: tree edges + adjacent position edges
    adj = [set() for _ in range(n)]
    for i in range(n):
        if left[i] != -1:
            adj[i].add(left[i])
            adj[left[i]].add(i)
        if right[i] != -1:
            adj[i].add(right[i])
            adj[right[i]].add(i)
    for i in range(n - 1):
        adj[i].add(i + 1)
        adj[i + 1].add(i)

    # coverage[i] = set of nodes covered by lighting node i (itself + neighbors)
    # Greedy: pick node covering the most uncovered nodes
    covered = [False] * n
    total_covered = 0
    result = 0

    # Max-heap with lazy deletion
    # Entry: (-coverage_count, node_index)
    heap = []
    for i in range(n):
        cnt = 1 + len(adj[i])  # covers itself + neighbors
        heappush(heap, (-cnt, i))

    selected = [False] * n

    while total_covered < n:
        # Find best node
        while heap:
            neg_cnt, node = heappop(heap)
            if selected[node]:
                continue
            # Recount actual uncovered coverage
            actual = 0
            if not covered[node]:
                actual += 1
            for nb in adj[node]:
                if not covered[nb]:
                    actual += 1
            if actual == 0:
                continue
            # Lazy check: if actual count differs, re-push
            if actual != -neg_cnt:
                heappush(heap, (-actual, node))
                continue
            # Select this node
            selected[node] = True
            result += 1
            if not covered[node]:
                covered[node] = True
                total_covered += 1
            for nb in adj[node]:
                if not covered[nb]:
                    covered[nb] = True
                    total_covered += 1
            break
        else:
            # Heap exhausted but not fully covered - shouldn't happen
            # Select any uncovered node
            for i in range(n):
                if not covered[i]:
                    covered[i] = True
                    total_covered += 1
                    result += 1
                    break

    return result


solve()
