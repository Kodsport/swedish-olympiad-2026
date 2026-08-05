#!/usr/bin/python3
# Enumerates both turning columns correctly, but finds the peak row by ternary
# search -- i.e. it assumes the cost is convex in the peak row, when it is in fact
# concave.  This *looks* unsound and is not, which is why it lives here rather than
# being something the data ought to kill.  Writing T = max(sy,gy), L = |c1-c2|+1:
#
#   staple(c1, c2, T + k) = staple(c1, c2, T) + k*(B - k),
#   B = f[c1] + f[c2] - L - 2T + 1,   0 <= k <= R-1-T
#
# is a downward parabola, so the minimum over the range is at an end and a ternary
# search may discard the winning one.  What saves this is that the probes are
# exactly symmetric: m1 + m2 == lo + hi identically, so for any concave f
#
#   f(m1) <= f(m2)  <=>  f(lo) <= f(hi)
#
# (both sides have the sign of -(m1-m2)*(lo+hi-2v) around the vertex v).  Each
# comparison therefore reports which *end* of the current range is better, and the
# branch taken keeps that end as an endpoint of the next range -- so the winning
# end survives every iteration and the trailing brute force over [lo, hi] hits it.
# Concavity also gives f(x) >= min(f(lo), f(hi)) in between, so nothing better is
# ever thrown away.
#
# Note what this does *not* rely on: the search does make real decisions on inputs
# where the top row wins (g2-peak-* / g3-peak-*, where the range is up to C-2 rows
# long and B = n-1), and the trailing brute force does not cover those on its own.
# The symmetry is the whole argument -- moving either probe, e.g. to lo + d//4 and
# lo + 3*d//4, breaks it (see partially_accepted/ternary_quartile.py).
#
# Verified against data_generation/ref.py exhaustively for R <= 6, C <= 3, and
# against nils.py over ~220k targeted inputs sweeping R-1-T in [0,40] with
# flat/near-flat/cheap-column/random profiles.

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

# peak exactly at max(sy, gy)
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
            m1 = lo + (hi - lo) // 3
            m2 = hi - (hi - lo) // 3
            if staple(c1, c2, m1) <= staple(c1, c2, m2):
                hi = m2 - 1
            else:
                lo = m1 + 1
        for t in range(lo, hi + 1):
            ans = min(ans, staple(c1, c2, t))

print(ans)
