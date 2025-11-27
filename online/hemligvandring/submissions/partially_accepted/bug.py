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




# Find smallest and largest x among all candidates
xlo = min(candidates[0][0][0],candidates[1][0][0],candidates[2][0][0],candidates[3][0][0],candidates[0][1][0],candidates[1][1][0],candidates[2][1][0],candidates[3][1][0])  
xhi = max(candidates[0][0][0],candidates[1][0][0],candidates[2][0][0],candidates[3][0][0],candidates[0][1][0],candidates[1][1][0],candidates[2][1][0],candidates[3][1][0])

# Find smallest and largest y among all candidates
ylo = min(candidates[0][0][1],candidates[1][0][1],candidates[2][0][1],candidates[3][0][1],candidates[0][1][1],candidates[1][1][1],candidates[2][1][1],candidates[3][1][1])
yhi = max(candidates[0][1][1],candidates[1][1][1],candidates[2][1][1],candidates[3][1][1],candidates[0][0][1],candidates[1][0][1],candidates[2][0][1],candidates[3][0][1])


# We assume 
p1 = candidates[0]
p2 = candidates[1]

p3 = candidates[2]
p4 = candidates[3]

B = [[*row[:]] for row in A]

for a,b in [p1,p2,p3,p4]:

    for point in [a,b]:

        if point[0] == xlo and point[1] == ylo:
            x,y = point        
            break
        if point[0] == xlo and point[1] == yhi:
            x,y = point
            break
        if point[0] == xhi and point[1] == ylo:
            x,y = point
            break
        if point[0] == xhi and point[1] == yhi:
            x,y = point     
            break

    else:
        continue
    break

B[x-1][y-1] = "#"
B[x-1][y+1] = "#"
B[x-1][y] = "#"
B[x][y+1] = "#"
B[x][y-1] = "#"
B[x+1][y-1] = "#"
B[x+1][y+1] = "#"
B[x+1][y] = "#"

xx,yy = x,y

ma,mb = a,b
x1,y1 = ma
x2,y2 = mb



fx,fy = xx,yy
for a,b in [p1,p2,p3,p4]:
    for point,notpoint in [(a,b),(b,a)]:
        x,y = point
        if (x,y) == (x1,y2):
            if x < x2:
                B[x+1][y] = "#"
            else:
                B[x-1][y] = "#"

            if y < y1:
                B[x][y+1] = "#"
            else:
                B[x][y-1] = "#" 

        if (x,y) == (x2,y1):
            if x < x1:
                B[x+1][y] = "#"
            else:
                B[x-1][y] = "#"
            if y < y2:
                B[x][y+1] = "#"
            else:
                B[x][y-1] = "#" 

        if abs(x-xx) + abs(y-yy) > abs(fx-xx) + abs(fy-yy):
            fx,fy = x,y
            ox,oy = notpoint

if ox < fx:
    B[fx-1][fy] = "#"
else:
    B[fx+1][fy] = "#"
if oy < fy:
    B[fx][fy-1] = "#"
else:
    B[fx][fy+1] = "#"


print("?")
for row in B:
    print("".join(row))
ret = int(input()) 

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

#print(distsmap2)

candidates = [(a,b) for a,b in candidates if distsmap3[b][a[0]][a[1]] == ret]

#assert(len(candidates) == 1)
a,b = candidates[0]
print("!", a[0], a[1], b[0], b[1])
