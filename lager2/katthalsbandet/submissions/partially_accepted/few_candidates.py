# Only try O(1) "smart" candidate rotations instead of all N.
# Candidates: r=0, r=argmin(P), r=argmax(P), r=argmin(unmatched).
import sys
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

def unmatched(r):
    mp = smin[r] - P[r]
    if r > 0:
        mp = min(mp, S - P[r] + pmin[r])
    return S + 2 * max(0, -mp)

# Candidate rotations
candidates = set()
candidates.add(0)

# argmin(P) - minimizes removals for equal parens
best_p, best_r = P[0], 0
for r in range(1, n):
    if P[r] < best_p:
        best_p = P[r]
        best_r = r
candidates.add(best_r)

# argmax(P) - minimizes removals when S < 0
best_p, best_r = P[0], 0
for r in range(1, n):
    if P[r] > best_p:
        best_p = P[r]
        best_r = r
candidates.add(best_r)

# argmin(unmatched) - the rotation minimizing removals
best_u, best_r = unmatched(0), 0
for r in range(1, n):
    u = unmatched(r)
    if u < best_u:
        best_u = u
        best_r = r
candidates.add(best_r)

print(min(cost(r) for r in candidates))
