# Try every K-th rotation where K = sqrt(N), instead of all N rotations.
# Misses optimal rotation if it falls between sample points.
import sys
from math import isqrt
input = sys.stdin.readline

n, a, b = map(int, input().split())
s = input().strip()

P = [0] * (n + 1)
for i in range(n):
    P[i + 1] = P[i] + (1 if s[i] == '(' else -1)
S = P[n]

# suffix min (inclusive)
smin = [0] * (n + 1)
smin[n] = P[n]
for i in range(n - 1, -1, -1):
    smin[i] = min(P[i], smin[i + 1])

# prefix min excluding P[0]
pmin = [float('inf')] * (n + 1)
for i in range(1, n + 1):
    pmin[i] = min(pmin[i - 1], P[i])

def cost(r):
    mp = smin[r] - P[r]
    if r > 0:
        mp = min(mp, S - P[r] + pmin[r])
    unmatched = S + 2 * max(0, -mp)
    return b * r + a * unmatched

K = max(1, isqrt(n))
best = float('inf')
for r in range(0, n, K):
    best = min(best, cost(r))
print(best)
