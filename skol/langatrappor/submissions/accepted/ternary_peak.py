#!/usr/bin/python3
# Enumerates both turning columns correctly, but finds the peak row by ternary
# search -- i.e. it assumes the cost is convex in the peak row, when it is in fact
# concave.  This *looks* unsound and is not, which is why it lives here rather than
# being something the data ought to kill.  Writing T = max(sy,gy):
#
#   delta(t -> t+1) = (f[c1] - (t+1)) + (f[c2] - (t+1)) - (L-2),  L = |c1-c2|+1
#
# is decreasing in t, so the cost is concave and its minimum over [T, R-1] sits at
# an end -- a ternary search may discard the winning one.  But f_i >= R-1 forces
# the concave maximum to t* >= R-6, so the cost is monotonically increasing over
# any interval long enough for the search to make a real decision, and it
# correctly walks to T.  Conversely the top row only wins when R-1-T is single
# digits, and then hi-lo is small enough that the trailing brute force covers it.
#
# Verified against data_generation/ref.py over ~1400 targeted inputs sweeping
# R-1-T in [0,24] with flat/near-flat/cheap-column/random profiles.  If this ever
# starts failing, the interesting question is which of the two bounds above broke.

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
