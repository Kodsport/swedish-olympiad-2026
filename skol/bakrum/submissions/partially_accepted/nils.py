#!/usr/bin/env python3

import random

def can_reach(x,y,v):
    # can you write vector (x,y) as linear combination of vectors in v
    if x == y == 0:
        return True
    if len(v) == 0:
        return False
    for _ in range(100):
        L = [x,y]
        i = random.randrange(0, len(v))
        j = random.randrange(0, 2)
        if v[i][j] != 0:
            r = L[j] // v[i][j]
            x2 = x-r*v[i][0]
            y2 = y-r*v[i][1]
            if abs(x2)+abs(y2) < abs(x)+abs(y):
                x = x2
                y = y2
        if x == 0 and y == 0:
            return True
    for _ in range(1000):
        L = [x,y]
        i = random.randrange(0, len(v))
        j = random.randrange(0, 2)

        if random.randint(0,4) != 4:
            if v[i][j] != 0:
                r = L[j] // v[i][j]
                x2 = x-r*v[i][0]
                y2 = y-r*v[i][1]
                if abs(x2)+abs(y2) < abs(x)+abs(y):
                    x = x2
                    y = y2
        else:
            x = x+(1-2*j)*v[i][0]
            y = y+(1-2*j)*v[i][1]
        if x == 0 and y == 0:
            return True
    return False

dx = [0,0,1,-1]
dy = [1,-1,0,0]

n,m = map(int,input().split())
q=5
D = 10 # make this many copies of the grid
grid = []

for r in range(n):
    grid.append(input())

comp_big = [[-1]*(D*m) for _ in range(D*n)]
c = 0
for i in range(n*D):
    for j in range(m*D):
        if comp_big[i][j] == -1 and grid[i%n][j%m] != '#':
            # BFS
            Q = [(i, j)]
            p = 0
            while p < len(Q):
                i2, j2 = Q[p]
                if comp_big[i2][j2] == -1:
                    comp_big[i2][j2] = c
                    for x in range(4):
                        i3 = (i2+dx[x])%(n*D)
                        j3 = (j2+dy[x])%(m*D)
                        if comp_big[i3][j3] == -1 and grid[i3%n][j3%m] != '#':
                            Q.append((i3,j3))
                p += 1
            c += 1

comp_small = [[-1]*(m) for _ in range(n)]
c = 0
for i in range(n):
    for j in range(m):
        if comp_small[i][j] == -1 and grid[i][j] != '#':
            # BFS
            Q = [(i, j)]
            p = 0
            while p < len(Q):
                i2, j2 = Q[p]
                if comp_small[i2][j2] == -1:
                    comp_small[i2][j2] = c
                    for x in range(4):
                        i3 = (i2+dx[x])%(n)
                        j3 = (j2+dy[x])%(m)
                        if comp_small[i3][j3] == -1 and grid[i3][j3] != '#':
                            Q.append((i3,j3))
                p += 1
            c += 1


small_vectors = []
for vi in range(c):
    i = -1
    j = -1
    for i2 in range(n):
        for j2 in range(m):
            if comp_small[i2][j2] == vi:
                i = i2
                j = j2
                break
    sv = []
    for vx in range(D):
        for vy in range(D):
            if comp_big[i+vx*n][j+vy*m] == comp_big[i][j]:
                sv.append((vx,vy))
    small_vectors.append(sv)


for _ in range(q):
    j1, i1, j2, i2 = map(int, input().split())
    # no endpoints on blocked squares
    assert comp_small[i1%n][j1%m] != -1 and comp_small[i2%n][j2%m] != -1
    if comp_small[i1%n][j1%m] == comp_small[i2%n][j2%m]:

        gx = (i2-i1) // n
        gy = (j2-j1) // m

        if can_reach(gx, gy, small_vectors[comp_small[i1%n][j1%m]]):
            print("Ja")
        else:
            print("Nej")
    else:
        # different small components
        print("Nej")
    

