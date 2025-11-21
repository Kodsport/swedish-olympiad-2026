#!/usr/bin/env python3

dx = [0,0,1,-1]
dy = [1,-1,0,0]

n,m = map(int,input().split())
q = 5
D = 10 # make this many copies of the grid
grid = []

for r in range(n):
    grid.append(input())

comp = [[-1]*(D*m) for _ in range(D*n)]
c = 0

for i in range(n*D):
    for j in range(m*D):
        if comp[i][j] == -1 and grid[i%n][j%m] != '#':
            # BFS
            Q = [(i, j)]
            p = 0
            while p < len(Q):
                i2, j2 = Q[p]
                if comp[i2][j2] == -1:
                    comp[i2][j2] = c
                    for x in range(4):
                        i3 = (i2+dx[x])%(n*D)
                        j3 = (j2+dy[x])%(m*D)
                        if comp[i3][j3] == -1 and grid[i3%n][j3%m] != '#':
                            Q.append((i3,j3))
                p += 1
            c += 1

for _ in range(q):
    j1, i1, j2, i2 = map(int, input().split())
    i1 %= (n*D)
    j1 %= (m*D)
    i2 %= (n*D)
    j2 %= (m*D)
    assert comp[i1][j1] != -1 and comp[i2][j2] != -1
    if comp[i1][j1] == comp[i2][j2]:
        print("Ja")
    else:
        print("Nej")
    

