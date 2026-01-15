#!/usr/bin/python3
n,q=map(int,input().split())

countries = [None] * n
for i in range(n):
    a = list(map(int,input().split()))[1:]
    countries[i] = a

for i in range(q):
    x = int(input())
    best = (0,n)
    for j in range(n):
        cnt = sum(1 if v >= x else 0 for v in countries[j])
        best = max(best, (cnt, j+1))
    print(best[1])
