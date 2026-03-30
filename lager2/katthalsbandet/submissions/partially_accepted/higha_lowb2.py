# @EXPECTED_GRADES@ WA WA WA WA AC AC WA WA
# Greedy removal (first excess ) from start, excess ( from end) is suboptimal
# for unequal parens - a different removal choice can reduce rotation cost.

from collections import Counter

n,a,b = map(int,input().split())
s = input()

A = [0]*n
for i in range(n):
    A[i] = 2 if s[i] == "(" else -2

l = s.count("(")
h = s.count(")")
ans = (max(l,h)-min(l,h))*a

if h > l:
    d = h-l
    B = []
    for x in A:
        if x == -2 and d:
            d -= 1
        else:
            B.append(x)
    A = B[:]
    n = len(A)
elif l > h:
    d = l-h
    B = []
    for x in A:
        if x == 2 and d:
            d -= 1
        else:
            B.append(x)
    A = B[:]
    n = len(A)

occ = [0]
for x in A:
    occ.append(occ[-1] + (x//2))
    occ.append(occ[-1] + (x//2))

moves = occ.index(min(occ))//2

best = b*moves

print(best+ans)
