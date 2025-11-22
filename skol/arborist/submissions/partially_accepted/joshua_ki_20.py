#!/usr/bin/python3
n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

coords.sort()

ans = 0
while coords:
    ans += 2*coords[-1]
    coords.pop()
    if coords and k >= 40:
        coords.pop()

print(2*n+ans)
