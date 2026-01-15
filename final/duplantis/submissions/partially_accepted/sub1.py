#!/usr/bin/python3

n,v = map(int,input().split())

A = list(map(int,input().split()))
print(n)
ans = [A[-1]] * n
print(*ans)
