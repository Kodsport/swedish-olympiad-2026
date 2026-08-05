#!/usr/bin/python3
"""Knife-edge generator for the peak row of "Langa trappor".

For a fixed pair of turning columns (c1, c2) the cost of the staple walk

    s -> c1 (row T) -> up to row T+k -> across to c2 -> down to g

is exactly

    cost(T + k) = cost(T) + k*(B - k),   B = f[c1] + f[c2] - L - 2T + 1,

where T = max(sy, gy), L = |c1 - c2| + 1 and 0 <= k <= n = R-1-T.  That is a
concave parabola, so the best peak row is always an end of the range -- but
*which* end flips exactly at B = n, and a search over the peak row decides it by
comparing two probes k1 < k2, whose sign is sign((k1-k2) * (B - k1 - k2)).  A
search is therefore right precisely when its probes satisfy k1 + k2 == n; one
whose probes land s rows short returns the wrong end for every B in
[n-s, n-1].  Only B = n-1 pins that down for every s at once, and that is what
this generator builds: the top row beats row T by a single unit.

The top row is the end that matters.  Row T is covered for free by trying each
turning column on its own (equivalently, by the c1 == c2 pairs, which have
B = 2n and so never lose row T), while nothing but the peak-row search covers
the top row.

B = n-1 needs the turning pair to span exactly n+2 columns, so the cheap block
is that wide.  `c` pads the grid out with f_i = 10^9 walls on either side of the
block, so a solution has to find the block instead of reading it off the edges of
the grid.
"""

import sys

MAXF = 10**9


def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default


r = int(cmdlinearg('r'))
c = int(cmdlinearg('c'))
gap = int(cmdlinearg('gap'))
drop = int(cmdlinearg('drop', 0))
swap = int(cmdlinearg('swap', 0))

block = gap + 2
assert block <= c, f"gap={gap} needs c >= {block}, got {c}"

lo = (c - block) // 2
hi = lo + block - 1
f = [r - 1 if lo <= j <= hi else MAXF for j in range(c)]

sy = r - 1 - gap
gy = sy - drop
assert gy >= 0, f"gap={gap} drop={drop} does not fit in r={r}"

# The design invariant: the ends of the cheap block are the turning pair that
# wins, and their parabola sits exactly one unit past the flip.  If this breaks,
# the case has stopped testing what it was written to test.
B = f[lo] + f[hi] - block - 2 * sy + 1
assert B == gap - 1, f"B={B}, expected n-1 = {gap - 1}"

if swap:
    sx, gx = hi, lo
else:
    sx, gx = lo, hi

print(r, c)
print(*f)
print(sx, sy)
print(gx, gy)
