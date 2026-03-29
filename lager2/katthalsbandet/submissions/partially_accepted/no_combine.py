# Never combines rotation and removal optimally.
# Option A: find cheapest rotation achieving minimum removals (= |S|).
# Option B: don't rotate, remove all unmatched at r=0.
# Output min(A, B).
import sys
input = sys.stdin.readline

n, a, b = map(int, input().split())
s = input().strip()

P = [0] * (n + 1)
for i in range(n):
    P[i + 1] = P[i] + (1 if s[i] == '(' else -1)
S = P[n]
excess = abs(S)

# Option B: no rotation, stack-based matching
stack = 0
unmatched = 0
for c in s:
    if c == '(':
        stack += 1
    else:
        if stack > 0:
            stack -= 1
        else:
            unmatched += 1
unmatched += stack
option_b = a * unmatched

# Option A: find cheapest rotation where removals = excess (minimum possible)
# A rotation r achieves min removals iff min_prefix_rotated >= min(0, S)
threshold = min(0, S)

# suffix min of P[r+1..n]
smin = [0] * (n + 1)
smin[n] = P[n]
for i in range(n - 1, -1, -1):
    smin[i] = min(P[i + 1], smin[i + 1])

# prefix min of P[1..r]
pmin = [float('inf')] * (n + 1)
for i in range(1, n + 1):
    pmin[i] = min(pmin[i - 1], P[i])

option_a = float('inf')
for r in range(n):
    if r == 0:
        if min(P) >= threshold:
            option_a = a * excess
            break
    else:
        ok = True
        # Check first half: P[r+1..N] - P[r] >= threshold
        if smin[r] - P[r] < threshold:
            ok = False
        # Check second half: P[j] + S - P[r] >= threshold for j=1..r
        if r >= 1 and pmin[r] + S - P[r] < threshold:
            ok = False
        # Check P'[N-r] = S - P[r] >= threshold
        if S - P[r] < threshold:
            ok = False
        if ok:
            option_a = b * r + a * excess
            break

print(min(option_a, option_b))
