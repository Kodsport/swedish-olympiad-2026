#!/usr/bin/python3
# Routes everything through the globally cheapest column (smallest f_i), on the
# theory that the cheapest column is always the one worth climbing in.  Ignores
# that reaching a distant column costs a horizontal cell per step, so it is killed
# wherever the detour outweighs the cheaper climb -- and, symmetrically, wherever
# staying in an expensive nearby column is better.

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

best = min(range(C), key=lambda i: F[i])

ans = (vertical(best, sy, gy) + horisontal(sx, best, sy)
       + horisontal(gx, best, gy) - f(sy, best) - f(gy, best))

temp = (horisontal(sx, best, sy) + vertical(best, sy, R - 1)
        + vertical(best, R - 1, gy) + horisontal(best, gx, gy))
temp -= f(sy, best) + f(R - 1, best) + f(gy, best)
ans = min(ans, temp)

print(ans)
