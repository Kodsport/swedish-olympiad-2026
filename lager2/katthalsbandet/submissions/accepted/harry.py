from collections import deque

n,a,b = map(int,input().split())
s = [*input()]

moves = n-1

newA = s[moves:] + s[:moves]

inds = [-1]*(n+n-1)
stack = []
removed = []
for i,x in enumerate(newA):
    if x == "(":
        stack.append(i+n-1)
    else:
        if stack:
            j = stack.pop()
            inds[i+n-1] = j
            inds[j] = i+n-1
        else:
            removed.append(i+n-1)

#removed += stack

removecnt = len(removed) + len(stack)
best = b*moves + removecnt*a

removed = removed[::-1]

for i in range(1,n):
    temp = b*(moves-i)

    ind = n+n-1  -i
    newind = n-1-i
    if newA[-i] == "(":
        if removed:
            j = removed.pop()
            removecnt -= 2
            inds[newind] = j
            inds[j] = newind
    else:
        if inds[ind] == -1:
            removed.append(newind)
        else:
            match = inds[ind]
            inds[ind] = -1
            inds[match] = -1
            removecnt += 2
            removed.append(newind)

    best = min(temp + a*removecnt, best)

print(best)