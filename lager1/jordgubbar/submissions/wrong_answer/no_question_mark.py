#!/usr/bin/python3

n, m = map(int, input().split())

ans = [1] * n

s = m-1

for i in range(n)[::-1]:
    print(i+1, s)
    result = input()
    if result == "kvar":
        ans[i] = s+1
    else:
        ans[i] = -1
        s -= 1
    if s == 0:
        break

print('!', *ans)
