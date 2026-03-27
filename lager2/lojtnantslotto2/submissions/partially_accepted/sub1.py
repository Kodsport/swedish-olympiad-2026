#!/usr/bin/python3
print("YES")
n = int(input())
for i in range(n):
    row = []
    for j in range(n):
        if i == j:
            row.append('1')
        else:
            row.append('0')
    print("".join(row))

