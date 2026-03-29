# @EXPECTED_GRADES@ WA WA WA WA WA AC WA WA
# Group 5: a=10^9, b=1, equal parens. Removal is too expensive, never remove.
# Find the smallest rotation that makes the string balanced (0 removals).
# Valid rotation r iff P[r] = min(P), where P is the prefix sum with +1/(, -1/).
n, a, b = map(int, input().split())
s = input()

P = [0] * (n + 1)
for i in range(n):
    P[i + 1] = P[i] + (1 if s[i] == '(' else -1)

min_val = min(P[r] for r in range(n))
for r in range(n):
    if P[r] == min_val:
        print(r * b)
        break
