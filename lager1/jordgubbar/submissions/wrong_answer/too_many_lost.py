#!/usr/bin/python3

n, m = map(int, input().split())
ans = [1] * n

for i in range(n):
    print('?', i+1, m)
    result = input()
    ans[i] = -1

print("!",*ans)

