#!/usr/bin/env python3
# @EXPECTED_GRADES@ WA WA WA WA WA

import sys


def main():
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))

    # Build visibility matrix in O(n^2)
    can_see = [[False] * n for _ in range(n)]
    for i in range(n):
        can_see[i][i] = True
        mx = 0
        for j in range(i + 1, n):
            if mx < min(p[i], p[j]):
                can_see[i][j] = True
                can_see[j][i] = True
            mx = max(mx, p[j])

    # INCORRECT DP: assumes only adjacent lit lighthouses cover the gap between them.
    # Fails when a distant lit lighthouse illuminates positions across closer lit ones.
    INF = float("inf")
    dp = [INF] * n

    for j in range(n):
        ok = True
        for k in range(j):
            if not can_see[j][k]:
                ok = False
                break
        if ok:
            dp[j] = 1

        for i in range(j):
            if dp[i] >= dp[j]:
                continue
            ok = True
            for k in range(i + 1, j):
                if not can_see[i][k] and not can_see[j][k]:
                    ok = False
                    break
            if ok:
                dp[j] = min(dp[j], dp[i] + 1)

    ans = INF
    for j in range(n):
        if dp[j] >= ans:
            continue
        ok = True
        for k in range(j + 1, n):
            if not can_see[j][k]:
                ok = False
                break
        if ok:
            ans = dp[j]

    print(ans)


if __name__ == "__main__":
    main()
