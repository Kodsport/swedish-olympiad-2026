#!/usr/bin/python3

n = int(input())
A = [input() for _ in range(n)]

def queryT1(A):
    n = len(A)
    B = [[*row] for row in A]
    for i in range(0,n,2):
        if i%4:
            B[i] = ["#"]*n
            B[i][-1] = "."
        else:
            B[i] = ["#"]*n
            B[i][0] = "."
    return ["".join(row) for row in B]

def queryT2(A):
    n = len(A)
    B = [[*row] for row in A]
    for i in range(0,n,2):
        if i%4:
            for j in range(n-1):
                B[j][i] = "#"
        else:
            for j in range(1,n):
                B[j][i] = "#"

    return ["".join(row) for row in B]

B = queryT1(A)
print("?")
for row in B:
    print(row)
firstval = int(input())

B = queryT2(A)
print("?")
for row in B:
    print(row)
secondval = int(input())


houses = [(i, j) for i in range(n) for j in range(n) if A[i][j] == 'H']

candidates = [(a, b) for a in houses for b in houses if a < b and a[0] != b[0] and a[1] != b[1]]

B = queryT1(A)
distsmap1 = {}
for sx,sy in houses:
    BFS = [(sx,sy)]
    dists = [[-1]*n for _ in range(n)]
    dists[sx][sy] = 0

    for cx,cy in BFS:
        for dx,dy in [(-1,0),(0,-1),(0,1),(1,0)]:
            if 0 <= cx+dx < n and 0 <= cy+dy < n:
                if B[cx+dx][cy+dy] != "#" and dists[cx+dx][cy+dy] == -1:
                    dists[cx+dx][cy+dy] = dists[cx][cy] + 1
                    BFS.append((cx+dx,cy+dy))
    
    distsmap1[(sx,sy)] = [row[:] for row in dists]

B = queryT2(A)
distsmap2 = {}
for sx,sy in houses:
    BFS = [(sx,sy)]
    dists = [[-1]*n for _ in range(n)]
    dists[sx][sy] = 0

    for cx,cy in BFS:
        for dx,dy in [(-1,0),(0,-1),(0,1),(1,0)]:
            if 0 <= cx+dx < n and 0 <= cy+dy < n:
                if B[cx+dx][cy+dy] != "#" and dists[cx+dx][cy+dy] == -1:
                    dists[cx+dx][cy+dy] = dists[cx][cy] + 1
                    BFS.append((cx+dx,cy+dy))
    
    distsmap2[(sx,sy)] = [row[:] for row in dists]

#print(distsmap2)

candidates = [(a,b) for a,b in candidates if distsmap1[a][b[0]][b[1]] == firstval and distsmap2[b][a[0]][a[1]] == secondval]

#print(len(candidates))
#print(candidates)
assert(len(candidates) <= 4)

if len(candidates) == 1:
    a,b = candidates[0]
    print("!", a[0], a[1], b[0], b[1])
    exit()


if len(candidates) == 2:

    smallest = min(min(*candidates[0]),min(*candidates[1]))
    #print(smallest)


    x1,y1 = smallest
    B = [row[:] for row in A]
    for x in range(x1+2):
        B[x] = B[x][:y1+1] + "#" + B[x][y1+2:]

    B[x1+1] = B[x1+1][:y1+2].replace(".","#") + B[x1+1][y1+2:]
    print("?")
    for row in B:
        print(row)

    ret = int(input())

    if (ret == -1):
        if candidates[0][0] == smallest:
            a,b = candidates[0]
            x1,y1 = a
            x2,y2 = b
        else:
            a,b = candidates[1]
            x1,y1 = a
            x2,y2 = b
        print("!",x1,y1,x2,y2)
    else:
        if candidates[0][0] == smallest:
            a,b = candidates[1]
            x1,y1 = a
            x2,y2 = b
        else:
            a,b = candidates[0]
            x1,y1 = a
            x2,y2 = b
        print("!",x1,y1,x2,y2)

    exit()

k = len(candidates)
# print(k)
# print(candidates)
from itertools import permutations


houses = [a for a,b in candidates] + [b for a,b in candidates]
houses = list(set(houses))



# 0 - block 1
# 1 - halfblock 2
# 2 - halfblock 1
# 3 - nothing
for perm in permutations([*range(4)]):
    
    for mask in range(1<<2):
        B = [[*row] for row in A]
        for i in range(k):
            if perm[i] == 0:
                
                x,y = candidates[i][mask&1]
                
                B[x-1][y-1] = "#"
                B[x-1][y+1] = "#"
                B[x-1][y] = "#"
                B[x][y+1] = "#"
                B[x][y-1] = "#"
                B[x+1][y-1] = "#"
                B[x+1][y+1] = "#"
                B[x+1][y] = "#"

            elif perm[i] == 1:
                (x1,y1),(x2,y2) = candidates[i]
                
                if x1 < x2:
                    B[x1+1][y1] = "#"
                    B[x2-1][y2] = "#"
                else:
                    B[x1-1][y1] = "#"
                    B[x2+1][y2] = "#"

                if y1 < y2:
                    B[x1][y1+1] = "#"
                    B[x2][y2-1] = "#"
                else:
                    B[x1][y1-1] = "#"
                    B[x2][y2+1] = "#"
            elif perm[i] == 2:
                (x1,y1),(x2,y2) = candidates[i]

                if mask&2:
                    x1,y1 = candidates[i][1]
                    x2,y2 = candidates[i][0]
                
                if x1 < x2:
                    B[x1+1][y1] = "#"
                else:
                    B[x1-1][y1] = "#"

                if y1 < y2:
                    B[x1][y1+1] = "#"
                else:
                    B[x1][y1-1] = "#"

        # print("hm")
        # print(perm,mask)
        # for row in B:
        #     print("".join(row))

        # Do BFS and check if all candidates give differetn distances
        distsmap3 = {}
        for sx,sy in houses:
            BFS = [(sx,sy)]
            dists = [[-1]*n for _ in range(n)]
            dists[sx][sy] = 0

            for cx,cy in BFS:
                for dx,dy in [(-1,0),(0,-1),(0,1),(1,0)]:
                    if 0 <= cx+dx < n and 0 <= cy+dy < n:
                        if B[cx+dx][cy+dy] != "#" and dists[cx+dx][cy+dy] == -1:
                            dists[cx+dx][cy+dy] = dists[cx][cy] + 1
                            BFS.append((cx+dx,cy+dy))
            
            distsmap3[(sx,sy)] = [row[:] for row in dists]
        distset = set()
        for a,b in candidates:
            if distsmap3[a][b[0]][b[1]] in distset:
                break
            distset.add(distsmap3[a][b[0]][b[1]])

        if len(distset) == k:
            # Found a working configuration
            print("?")
            for row in B:
                print("".join(row))
            res = int(input())
            ans = []
            for i in range(k):
                a,b = candidates[i]
                dist = distsmap3[a][b[0]][b[1]]
                if dist == res:
                    ans.append((a,b))
            assert(len(ans) == 1)
            for a,b in ans:
                print("!", a[0], a[1], b[0], b[1])
            exit()