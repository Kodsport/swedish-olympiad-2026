#!/usr/bin/python3

r,c=map(int, input().split())
grid = [list(input()) for i in range(r)]
        
max_r=-1
max_c=-1
min_r = 100
min_c = 100
for i in range(r):
    for j in range(c):
        if grid[i][j] == '*':
            max_r = max(max_r, i)
            max_c = max(max_c, j)
            min_r = min(min_r, i)
            min_c = min(min_c, j)

grid[(max_r+min_r)//2][(max_c+min_c)//2]='X'
[print(''.join(row)) for row in grid]
