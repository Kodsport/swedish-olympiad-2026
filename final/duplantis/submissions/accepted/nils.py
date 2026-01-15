#!/usr/bin/python3

n,v = map(int,input().split())

A = list(map(int,input().split()))
suf_max = [-1] * n

for i in range(n)[::-1]:
    suf_max[i] = max(suf_max[i], A[i])
    if i > 0:
        suf_max[i-1] = suf_max[i]

ans = []
for i in range(n):
    if A[i] > v:
        ans.append(suf_max[i])
        v += 1

print(len(ans))
print(*ans)