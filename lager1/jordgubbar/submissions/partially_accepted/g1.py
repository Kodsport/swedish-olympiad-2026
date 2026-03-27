#!/usr/bin/python3

n, m = map(int, input().split())

ans = [1] * n
for i in range(n)[::-1]:
    print("?", i+1, 1)
    if input() == "borta":
        ans[i] = -1
        break
    else:
        ans[i] = 2
print("!",*ans)
