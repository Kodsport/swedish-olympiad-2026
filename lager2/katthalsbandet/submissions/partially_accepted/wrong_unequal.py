# Uses the equal-parens formula for ALL strings (wrong for unequal parens).
# For equal parens: removals(r) = 2*(P[r] - M) where M = global min(P). Correct.
# For unequal: uses |S| + 2*max(0, P[r] - M). Wrong because it should use
# per-rotation min instead of global min M.
import sys
input = sys.stdin.readline

n, a, b = map(int, input().split())
s = input().strip()

P = [0] * (n + 1)
for i in range(n):
    P[i + 1] = P[i] + (1 if s[i] == '(' else -1)
S = P[n]

M = min(P[r] for r in range(n + 1))

best = float('inf')
for r in range(n):
    removals = abs(S) + 2 * max(0, P[r] - M)
    cost = b * r + a * removals
    best = min(best, cost)
print(best)
