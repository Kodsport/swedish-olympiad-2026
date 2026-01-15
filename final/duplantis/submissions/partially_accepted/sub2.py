#!/usr/bin/python3

n,v = map(int,input().split())

A = list(map(int,input().split()))
ans = []

for i in range(n):
    if v < A[i]:
        ans.append(A[i])
        v += 1

print(len(ans))
print(*ans)
