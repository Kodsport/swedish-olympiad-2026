#!/usr/bin/python3
# Knows that the peak may be the top row, but only ever uses a *single* turning
# column: it goes up and back down in the same column instead of crossing the top
# row from one column to another.  Correct for C = 2, killed by the *-climb cases,
# where the whole point is to cross the cheap top row between two columns.

def f(r, c):
    return F[c] - r

def horisontal(c1, c2, r):
    if c1 > c2:
        (c1, c2) = (c2, c1)
    res = 0
    while c1 <= c2:
        res += f(r, c1)
        c1 += 1
    return res

def vertical(c, r1, r2):
    if r1 > r2:
        (r1, r2) = (r2, r1)
    d = r2 - r1 + 1
    return d * F[c] - d * r1 - (d**2 - d) // 2


R, C = map(int, input().split())
F = list(map(int, input().split()))
sx, sy = map(int, input().split())
gx, gy = map(int, input().split())
if sy < gy:
    (sx, sy, gx, gy) = (gx, gy, sx, sy)

ans = 10**20

for i in range(C):
    temp = (vertical(i, sy, gy) + horisontal(sx, i, sy) + horisontal(gx, i, gy)
            - f(sy, i) - f(gy, i))
    ans = min(ans, temp)

for c in range(C):
    temp = (horisontal(sx, c, sy) + vertical(c, sy, R - 1)
            + vertical(c, R - 1, gy) + horisontal(c, gx, gy))
    temp -= f(sy, c) + f(R - 1, c) + f(gy, c)
    ans = min(ans, temp)

print(ans)
