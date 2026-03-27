#!/usr/bin/python3
n = int(input())
P = [int(x)-1 for x in input().split()]
grid = [['1']*n for _ in range(n)]
for i in range(n):
    grid[i][i] = '0'
    grid[i][P[i]] = '0'
    if i != P[i] and i == P[P[i]] and i > P[i]:
        yes = 0
        for j in range(n):
            if grid[j][i] != grid[j][P[i]]:
                yes = 1
                break
            if grid[j][i] == '1' and grid[j][P[i]] == '1':
                x,y = j,i
                while grid[x][y] == '1':
                    grid[x][y] = '0'
                    x,y = y,P[x]
                if grid[j][P[i]] == '1':
                    yes = 1
                    break
        if yes == 0:
            print("NO")
            exit()

print("YES")
for i in range(n):
    print("".join(grid[i]))
