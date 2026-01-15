#!/usr/bin/python3
from bisect import bisect_left
n,q=map(int,input().split())

countries = [None] * n
for i in range(n):
    a = list(map(int,input().split()))[1:]
    a.sort()
    countries[i] = a

for i in range(q):
    x = int(input())
    best = (0,n)
    for j in range(n):
        cnt = len(countries[j]) - bisect_left(countries[j], x)
        best = max(best, (cnt, j+1))
    print(best[1])
