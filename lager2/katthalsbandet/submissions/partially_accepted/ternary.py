# Ternary search on cost(r), assuming cost is unimodal (it's not in general).
# Computes cost(r) correctly, but only evaluates O(log N) rotations.
import sys
input = sys.stdin.readline

n, a, b = map(int, input().split())
s = input().strip()

P = [0] * (n + 1)
for i in range(n):
    P[i + 1] = P[i] + (1 if s[i] == '(' else -1)
S = P[n]

# suffix min (inclusive): smin[r] = min(P[r], P[r+1], ..., P[n])
smin = [0] * (n + 1)
smin[n] = P[n]
for i in range(n - 1, -1, -1):
    smin[i] = min(P[i], smin[i + 1])

# prefix min excluding P[0]: pmin[r] = min(P[1], ..., P[r]) for r >= 1
pmin = [float('inf')] * (n + 1)
for i in range(1, n + 1):
    pmin[i] = min(pmin[i - 1], P[i])

def cost(r):
    mp = smin[r] - P[r]
    if r > 0:
        mp = min(mp, S - P[r] + pmin[r])
    unmatched = S + 2 * max(0, -mp)
    return b * r + a * unmatched

# Ternary search assuming unimodal
lo, hi = 0, n - 1
while hi - lo > 2:
    m1 = lo + (hi - lo) // 3
    m2 = hi - (hi - lo) // 3
    if cost(m1) < cost(m2):
        hi = m2
    else:
        lo = m1

best = min(cost(r) for r in range(lo, hi + 1))
print(best)
