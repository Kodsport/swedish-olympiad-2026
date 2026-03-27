#!/usr/bin/env python3
# @EXPECTED_GRADES@ AC AC TLE TLE TLE

import sys


def main():
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))

    INF = float("inf")

    # dp[l][r][el][er][ol][or_] = min lighthouses to illuminate [l..r]
    # el/er: external illumination from parent on left/right
    # ol/or_: whether subtree provides outward illumination left/right
    # Based on Cartesian tree interval DP: max element splits into independent halves
    dp = [
        [[[[[INF] * 2 for _ in range(2)] for _ in range(2)] for _ in range(2)] for _ in range(n)]
        for _ in range(n)
    ]

    for length in range(1, n + 1):
        for l in range(n - length + 1):
            r = l + length - 1

            # Find argmax in [l, r]: O(n) per interval, giving O(n^3) total
            m = l
            for i in range(l + 1, r + 1):
                if p[i] > p[m]:
                    m = i

            has_left = l <= m - 1
            has_right = m + 1 <= r

            for el in range(2):
                for er in range(2):
                    # Case 1: Light m
                    cost = 1
                    if has_left:
                        best = INF
                        for a in range(2):
                            for b in range(2):
                                best = min(best, dp[l][m - 1][el][1][a][b])
                        cost += best
                    if has_right:
                        best = INF
                        for a in range(2):
                            for b in range(2):
                                best = min(best, dp[m + 1][r][1][er][a][b])
                        cost += best
                    dp[l][r][el][er][1][1] = min(dp[l][r][el][er][1][1], cost)

                    # Case 2: Don't light m
                    if has_left:
                        left_states = []
                        for a in range(2):
                            for b in range(2):
                                c = dp[l][m - 1][el][0][a][b]
                                if c < INF:
                                    left_states.append((c, a, b))
                    else:
                        left_states = [(0, 0, 0)]

                    if has_right:
                        right_states = []
                        for a in range(2):
                            for b in range(2):
                                c = dp[m + 1][r][0][er][a][b]
                                if c < INF:
                                    right_states.append((c, a, b))
                    else:
                        right_states = [(0, 0, 0)]

                    for lc, lol, lor in left_states:
                        for rc, rol, ror in right_states:
                            if not (el or er or lor or rol):
                                continue
                            ol = lol if has_left else 0
                            or_ = ror if has_right else 0
                            total = lc + rc
                            if total < dp[l][r][el][er][ol][or_]:
                                dp[l][r][el][er][ol][or_] = total

    ans = INF
    for a in range(2):
        for b in range(2):
            ans = min(ans, dp[0][n - 1][0][0][a][b])
    print(ans)


if __name__ == "__main__":
    main()
