#!/usr/bin/python3
n,q=map(int,input().split())

countries = [None] * n
m = 0
for i in range(n):
    a = list(map(int,input().split()))[1:]
    m = max(m, max(a))
    countries[i] = a

seen = {}

for i in range(q):
    x = int(input())
    x = min(x, m+1)
    if x in seen:
        print(seen[x])
        continue
    best = (0,n)
    for j in range(n):
        cnt = sum(1 if v >= x else 0 for v in countries[j])
        best = max(best, (cnt, j+1))
    seen[x] = best[1]
    print(best[1])
