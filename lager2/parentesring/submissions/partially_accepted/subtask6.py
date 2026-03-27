# @EXPECTED_GRADES@ WA WA WA AC AC AC WA WA
# Group 6: a=10^9, b=1. Removal is expensive, so minimize removals first.
# Find the smallest rotation where removals = |l-h| (the theoretical minimum).
# Uses prefix sums: rotation r achieves minimum removals iff the rotated prefix
# sum never goes below min(0, l-h). O(N).
n, a, b = map(int, input().split())
s = input()

P = [0] * (n + 1)
for i in range(n):
    P[i + 1] = P[i] + (1 if s[i] == '(' else -1)

l = s.count('(')
h = s.count(')')
excess = abs(l - h)
threshold = min(0, P[n])

# suffix_min_exclusive[r] = min(P[r+1], ..., P[n])
smin = [0] * (n + 1)
smin[n] = P[n]
for r in range(n - 1, -1, -1):
    smin[r] = min(P[r + 1], smin[r + 1])

# prefix_min_inner[r] = min(P[1], ..., P[r])
pmin = [float('inf')] * (n + 1)
for r in range(1, n + 1):
    pmin[r] = min(pmin[r - 1], P[r])

best_r = 0
for r in range(n):
    if r == 0:
        if min(P) >= threshold:
            best_r = 0
            break
    else:
        if smin[r] >= P[r] + threshold and pmin[r] + P[n] >= P[r] + threshold:
            best_r = r
            break

print(excess * a + best_r * b)
