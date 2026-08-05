#!/usr/bin/python3
# nils.py with the "cross at the top row" case removed: assumes the walk never
# climbs above the higher of the two endpoints, so it only ever turns in a single
# column.  Correct for C = 2 (with only two cells on the crossing row, climbing
# can never pay for itself), wrong as soon as max(sy,gy) is within ~C rows of
# R-1 and crossing the cheap top row beats crossing at max(sy,gy).

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

print(ans)
