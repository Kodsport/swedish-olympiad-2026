#!/usr/bin/python3

n, m = map(int, input().split())

ans = [m] * n

for s in range(1, m):
    gone = 0
    for i in range(n)[::-1]:
        if ans[i] == -1:
            break
        print('?', i+1, s)
        result = input()
        if result == "borta":
            gone = i
            ans[gone] = -1
            break
    for i in range(gone):
        if ans[i] == m:
            ans[i] = s


print('!', *ans)
