#!/usr/bin/env python3
# @EXPECTED_GRADES@ WA WA WA WA WA
#
# Cheese: Greedy set cover on the visibility graph. Repeatedly lights the
# lighthouse covering the most uncovered positions. Finds a valid cover
# but not necessarily minimum, so the answer is >= optimal.

import sys
import heapq


def main():
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))

    if n == 1:
        print(1)
        return

    # Build Cartesian tree using monotonic stack (max-heap)
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

    # Build adjacency list: tree edges + adjacent position edges
    adj = [set() for _ in range(n)]
    for i in range(n):
        if left[i] != -1:
            adj[i].add(left[i])
            adj[left[i]].add(i)
        if right[i] != -1:
            adj[i].add(right[i])
            adj[right[i]].add(i)
        if i + 1 < n:
            adj[i].add(i + 1)
            adj[i + 1].add(i)

    # Convert to frozen lists for fast iteration
    adj_list = [list(s) for s in adj]

    # Coverage set for each node: {itself} + neighbors
    # uncov_count[i] = number of uncovered nodes in coverage_set[i]
    covered = [False] * n
    uncov_count = [1 + len(adj_list[i]) for i in range(n)]

    # Max-heap using negative counts (Python min-heap)
    heap = [(-uncov_count[i], i) for i in range(n)]
    heapq.heapify(heap)

    ans = 0
    total_covered = 0

    while total_covered < n:
        # Pop until we find a valid entry
        while heap:
            neg_cnt, x = heapq.heappop(heap)
            if -neg_cnt == uncov_count[x] and uncov_count[x] > 0:
                break
        else:
            break

        # Light node x
        ans += 1

        # Find newly covered nodes in coverage_set[x]
        newly_covered = []
        if not covered[x]:
            newly_covered.append(x)
        for nb in adj_list[x]:
            if not covered[nb]:
                newly_covered.append(nb)

        # Mark them as covered
        for m in newly_covered:
            covered[m] = True
        total_covered += len(newly_covered)

        # Update uncov_count for affected nodes
        for m in newly_covered:
            # m is now covered, so for every node that has m in its coverage set,
            # decrement their uncov_count. m's coverage set neighbors are adj[m],
            # plus m itself.
            uncov_count[m] -= 1
            if uncov_count[m] >= 0:
                heapq.heappush(heap, (-uncov_count[m], m))
            for nb in adj_list[m]:
                uncov_count[nb] -= 1
                if uncov_count[nb] >= 0:
                    heapq.heappush(heap, (-uncov_count[nb], nb))

    print(ans)


if __name__ == "__main__":
    main()
