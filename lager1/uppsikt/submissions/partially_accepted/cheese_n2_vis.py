#!/usr/bin/env python3
# @EXPECTED_GRADES@ AC AC AC TLE TLE
#
# Cheese: Computes ALL O(n^2) visibility pairs, then extracts the Cartesian
# tree from the visibility data. Strictly O(n^2) regardless of input.

import sys

INF = 10**9


def build_cartesian_tree(p):
    n = len(p)
    left = [-1] * n
    right = [-1] * n
    parent = [-1] * n

    nearest_taller_left = [-1] * n
    nearest_taller_right = [-1] * n

    # O(n^2) scan of all pairs - no early termination
    for i in range(n):
        mx = 0
        for j in range(i + 1, n):
            if mx < min(p[i], p[j]):
                # i and j can see each other
                if p[j] > p[i] and nearest_taller_right[i] == -1:
                    nearest_taller_right[i] = j
                if p[i] > p[j]:
                    nearest_taller_left[j] = i
            mx = max(mx, p[j])

    for i in range(n):
        lg = nearest_taller_left[i]
        rg = nearest_taller_right[i]
        if lg == -1:
            parent[i] = rg
        elif rg == -1:
            parent[i] = lg
        elif p[lg] < p[rg]:
            parent[i] = lg
        else:
            parent[i] = rg

    root = -1
    for node, par in enumerate(parent):
        if par == -1:
            root = node
        elif node < par:
            left[par] = node
        else:
            right[par] = node

    return root, left, right


def solve(left, right, root):
    order = []
    stack = [root]
    while stack:
        node = stack.pop()
        order.append(node)
        if left[node] != -1:
            stack.append(left[node])
        if right[node] != -1:
            stack.append(right[node])

    dp = [None] * len(left)
    for node in reversed(order):
        lnode = left[node]
        rnode = right[node]
        cur = [[[[INF] * 2 for _ in range(2)] for _ in range(2)] for _ in range(2)]

        for ext_left in range(2):
            for ext_right in range(2):
                cost = 1
                if lnode != -1:
                    best = INF
                    child = dp[lnode]
                    for has_left in range(2):
                        for has_right in range(2):
                            best = min(best, child[ext_left][1][has_left][has_right])
                    cost += best
                if rnode != -1:
                    best = INF
                    child = dp[rnode]
                    for has_left in range(2):
                        for has_right in range(2):
                            best = min(best, child[1][ext_right][has_left][has_right])
                    cost += best
                cur[ext_left][ext_right][1][1] = cost

                left_states = [(0, 0, 0)]
                if lnode != -1:
                    child = dp[lnode]
                    left_states = []
                    for has_left in range(2):
                        for has_right in range(2):
                            left_states.append(
                                (child[ext_left][0][has_left][has_right], has_left, has_right)
                            )

                right_states = [(0, 0, 0)]
                if rnode != -1:
                    child = dp[rnode]
                    right_states = []
                    for has_left in range(2):
                        for has_right in range(2):
                            right_states.append(
                                (child[0][ext_right][has_left][has_right], has_left, has_right)
                            )

                for left_cost, left_has_left, left_has_right in left_states:
                    for right_cost, right_has_left, right_has_right in right_states:
                        if left_cost >= INF or right_cost >= INF:
                            continue
                        if not (
                            ext_left or ext_right or left_has_right or right_has_left
                        ):
                            continue
                        out_left = left_has_left if lnode != -1 else 0
                        out_right = right_has_right if rnode != -1 else 0
                        cur[ext_left][ext_right][out_left][out_right] = min(
                            cur[ext_left][ext_right][out_left][out_right],
                            left_cost + right_cost,
                        )

        dp[node] = cur

    return min(
        dp[root][0][0][has_left][has_right]
        for has_left in range(2)
        for has_right in range(2)
    )


def main():
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))
    root, left, right = build_cartesian_tree(p)
    print(solve(left, right, root))


if __name__ == "__main__":
    main()
