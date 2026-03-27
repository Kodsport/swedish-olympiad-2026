#!/usr/bin/env python3
# @EXPECTED_GRADES@ WA WA WA WA WA
#
# Cheese: Solves minimum dominating set on the Cartesian tree only,
# ignoring non-tree adjacent edges. Overestimates the answer.

import sys

INF = 10**9


def main():
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))

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

    root = -1
    for i in range(n):
        if parent[i] == -1:
            root = i
            break

    # Iterative pre-order to get traversal order, then reverse for post-order
    order = []
    stk = [root]
    while stk:
        node = stk.pop()
        order.append(node)
        if left[node] != -1:
            stk.append(left[node])
        if right[node] != -1:
            stk.append(right[node])

    # 3-state tree DP for minimum dominating set on the tree
    # dp[v][0] = v is selected (lit)
    # dp[v][1] = v not selected, dominated by at least one child
    # dp[v][2] = v not selected, not yet dominated (needs parent)
    dp0 = [0] * n
    dp1 = [0] * n
    dp2 = [0] * n

    for node in reversed(order):
        children = []
        if left[node] != -1:
            children.append(left[node])
        if right[node] != -1:
            children.append(right[node])

        if not children:
            dp0[node] = 1
            dp1[node] = INF
            dp2[node] = 0
        else:
            cost0 = 1
            for c in children:
                cost0 += min(dp0[c], dp1[c], dp2[c])
            dp0[node] = cost0

            cost2 = 0
            for c in children:
                cost2 += min(dp0[c], dp1[c])
            dp2[node] = cost2

            has_selected = any(dp0[c] <= dp1[c] for c in children)
            if has_selected:
                dp1[node] = cost2
            else:
                min_extra = min(dp0[c] - dp1[c] for c in children)
                dp1[node] = cost2 + min_extra

    print(min(dp0[root], dp1[root]))


if __name__ == "__main__":
    main()
