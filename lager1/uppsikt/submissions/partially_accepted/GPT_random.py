#!/usr/bin/env python3
# @EXPECTED_GRADES@ AC AC AC AC TLE

import sys

INF = 10**9


def build_cartesian_tree(p):
    n = len(p)
    left = [-1] * n
    right = [-1] * n

    sys.setrecursionlimit(300000)

    def build(lo, hi):
        if lo > hi:
            return -1
        root = lo
        for i in range(lo + 1, hi + 1):
            if p[i] > p[root]:
                root = i
        left[root] = build(lo, root - 1)
        right[root] = build(root + 1, hi)
        return root

    return build(0, n - 1), left, right


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


def main() -> None:
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))

    root, left, right = build_cartesian_tree(p)
    print(solve(left, right, root))


if __name__ == "__main__":
    main()
