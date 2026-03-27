#!/usr/bin/python3

n, m = map(int, input().split())
ans = [1] * n

for i2 in range(m-1):
    print('?', n-i2, m)
    result = input()
    ans[n-i2-1] = -1

print("!",*ans)

