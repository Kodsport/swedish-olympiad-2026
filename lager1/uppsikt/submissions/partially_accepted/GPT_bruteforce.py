#!/usr/bin/env python3
# @EXPECTED_GRADES@ AC TLE TLE TLE TLE

import sys


def build_cover(p):
    n = len(p)
    cover = [[False] * n for _ in range(n)]
    for i in range(n):
        cover[i][i] = True
        best = 0
        for j in range(i + 1, n):
            if best < min(p[i], p[j]):
                cover[i][j] = True
                cover[j][i] = True
            best = max(best, p[j])
    return cover


def main() -> None:
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))

    cover = build_cover(p)
    best = n
    for mask in range(1 << n):
        bits = mask.bit_count()
        if bits >= best:
            continue
        lit = [False] * n
        for i in range(n):
            if (mask >> i) & 1:
                for j in range(n):
                    if cover[i][j]:
                        lit[j] = True
        if all(lit):
            best = bits

    print(best)


if __name__ == "__main__":
    main()
