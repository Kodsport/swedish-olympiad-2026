# @EXPECTED_GRADES@ WA WA WA WA AC WA AC WA
# Group 7: Equal parens, arbitrary a and b. O(N).
# For equal parens, removals at rotation r = 2*(P[r] - M) where M = min prefix sum.
# Minimize b*r + a*removals over all rotations.
n, a, b = map(int, input().split())
s = input()

P = [0] * (n + 1)
for i in range(n):
    P[i + 1] = P[i] + (1 if s[i] == '(' else -1)

M = min(P[r] for r in range(n))
best = float('inf')
for r in range(n):
    removals = 2 * (P[r] - M)
    cost = b * r + a * removals
    best = min(best, cost)
print(best)
