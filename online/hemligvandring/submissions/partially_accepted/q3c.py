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

def queryT2_spiral(A):
    n = len(A)

    # Step 1: everything becomes wall except houses
    B = []
    for i in range(n):
        row = []
        for j in range(n):
            if A[i][j] == 'H':
                row.append('H')
            else:
                row.append('#')
        B.append(row)

    # Spiral carve: start at (n-1, 1)
    x, y = n-2, 1

    # Initial carve lengths
    vx = n - 3
    vy = n - 3

    # Movement directions in spiral order:
    # up, right, down, left
    dirs = [(-1,0), (0,1), (1,0), (0,-1)]
    d = 0  # direction index
    first = True
    while vx > 0 and vy > 0:
        # choose number of steps for this leg
        steps = vx if d == 0 or d == 2 else vy

        if steps <= 0:
            d = (d + 1) % 4
            continue

        dx, dy = dirs[d]

        # Carve corridor for this leg
        for _ in range(steps):
            if 0 <= x < n and 0 <= y < n:
                if B[x][y] != 'H':   # don't touch houses
                    B[x][y] = '.'
            x += dx
            y += dy

            # If we step out or hit house, stop spiral
            if not (0 <= x < n and 0 <= y < n):
                break
        
        # #print("EERM")
        # for row in B:
        #     print("".join(row))

        # After carving:
        # reduce appropriate counter
        if first:
            first = False
        elif d == 0 or d == 2:
            vx -= 2
        else:
            vy -= 2

        # rotate direction
        d = (d + 1) % 4

    # Convert back to strings
    return ["".join(row) for row in B]





B = queryT1(A)
print("?")
for row in B:
    print(row)
firstval = int(input())

B = queryT2_spiral(A)
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

B = queryT2_spiral(A)
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
#print(firstval, secondval)
#print(candidates)
assert(len(candidates) <= 4)

#######
#H.H.H#
#.###.#
#H#H.H#
#.#####
#H.H.H.
#######

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
#print(candidates)
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