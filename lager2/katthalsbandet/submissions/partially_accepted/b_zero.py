# @EXPECTED_GRADES@ WA WA WA WA AC WA WA WA
# Group 4: b=0. Free rotation, so we can always rotate to match all pairs.
# Minimum removals = |count('(') - count(')')|. Cost = removals * a.
n, a, b = map(int, input().split())
s = input()
l = s.count('(')
h = s.count(')')
print(abs(l - h) * a)
