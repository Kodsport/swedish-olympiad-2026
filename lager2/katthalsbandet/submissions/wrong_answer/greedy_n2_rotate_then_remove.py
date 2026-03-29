# @EXPECTED_GRADES@ TLE WA WA WA WA WA WA TLE
# Group 3: N<=2000. Try all rotations, O(N) greedy matching per rotation. O(N^2) total.
n, a, b = map(int, input().split())
s = input() * 2

best = float('inf')
for start in range(n):
    cost = start * b
    stack = 0
    removals = 0
    for i in range(start, start + n):
        if s[i] == '(':
            stack += 1
        else:
            if stack > 0:
                stack -= 1
            else:
                removals += 1
    cost += (removals + stack) * a
    best = min(best, cost)
print(best)
