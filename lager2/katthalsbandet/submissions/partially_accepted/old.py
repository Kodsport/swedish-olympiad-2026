# @EXPECTED_GRADES@ WA WA WA WA AC AC WA WA

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
    for x in A[::-1]:
        if x == 2 and d:
            d -= 1
        else:
            B.append(x)
    A = B[::-1]
    n = len(A)

occ = [0]

for x in A:
    occ.append(occ[-1] + (x//2))
    occ.append(occ[-1] + (x//2))

firstind = [-1]*(4*n+4)

C = Counter(occ)

for i in range(len(occ)):
    if occ[i] > 0:
        continue
    if firstind[occ[i]] == -1 and occ[i]%2 == 0:
        firstind[occ[i]] = i//2

for i in range(-2*n,3):
    C[i] += C[i-2]


best = 1e18
for i in range(-2*n,2,2):
    if firstind[i] == -1:
        continue
    best = min(ans + C[i-1]*a + firstind[i]*b, best)


print(best)
