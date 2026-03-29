# @EXPECTED_GRADES@ AC WA WA WA WA WA WA WA
# Group 1: a=1, b=10^9. Rotation is too expensive, never rotate.
# Greedy stack matching gives minimum removals. O(N).
n, a, b = map(int, input().split())
s = input()
stack = 0
removals = 0
for c in s:
    if c == '(':
        stack += 1
    else:
        if stack > 0:
            stack -= 1
        else:
            removals += 1
print((removals + stack) * a)
