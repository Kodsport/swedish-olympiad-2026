# @EXPECTED_GRADES@ AC WA WA WA AC AC WA WA

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

moves = occ.index(min(occ))//2

# Find the first move point, and simulate backwards

newA = A[moves:] + A[:moves]

best = b*moves
stack = []
for i in range(1,moves+1):
    temp = b*(moves-i)

    if newA[-i] == -2:
        stack.append(1)
    else:
        stack.pop()
    #print(stack,temp,best)
    best = min(temp + a*len(stack)*2, best)

print(best+ans)
