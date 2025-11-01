#!/usr/bin/python3

n,m = map(int, input().split())

grid = []
for _ in range(n):
    grid.append(list(input()))

tingsplats_found = False
for i in range(n):
    for j in range(m):
        d = -1
        possible = True
        for i2 in range(n):
            for j2 in range(m):
                if grid[i2][j2] == '*':
                    d2 = abs(i2-i) + abs(j2-j)
                    if d == -1:
                        d = d2
                    if d != d2:
                        possible = False
        if possible:
            grid[i][j] = 'X'
            tingsplats_found = True

assert tingsplats_found

for i in range(n):
    print("".join(grid[i]))
