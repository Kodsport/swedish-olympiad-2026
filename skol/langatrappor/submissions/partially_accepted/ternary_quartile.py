#!/usr/bin/python3
# Same solution as accepted/ternary_peak.py, but the peak-row search probes the
# quartiles, m1 = lo + d//4 and m2 = lo + 3*d//4, instead of m1 = lo + d//3 and
# m2 = hi - d//3.
#
# The cost of the staple walk is concave in the peak row, so its minimum is at an
# end of the range, and comparing two probes k1 < k2 only reveals which end that
# is when k1 + k2 == hi + lo.  The accepted version gets that identity for free;
# quartiles get it only when 4 divides d, and lose the top row whenever the range
# length is 3 mod 4 or 1 mod 4.  d == 8 happens to be safe, which is why this
# passes every case with the endpoints eight rows from the top and needs the
# other gap lengths in data/gen_peak.py to be caught.
#
# C = 2 leaves no room to cross the top row at all, so group 1 still passes.

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


def staple(c1, c2, t):
    temp = (horisontal(sx, c1, sy) + vertical(c1, sy, t)
            + horisontal(c1, c2, t) + vertical(c2, t, gy)
            + horisontal(c2, gx, gy))
    return temp - f(sy, c1) - f(t, c1) - f(t, c2) - f(gy, c2)


for c1 in range(C):
    for c2 in range(C):
        lo, hi = sy, R - 1
        while hi - lo > 2:
            d = hi - lo
            m1 = lo + d // 4
            m2 = lo + 3 * d // 4
            if staple(c1, c2, m1) <= staple(c1, c2, m2):
                hi = m2 - 1
            else:
                lo = m1 + 1
        for t in range(lo, hi + 1):
            ans = min(ans, staple(c1, c2, t))

print(ans)
