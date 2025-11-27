#!/usr/bin/python3

class UnionFind:
    def __init__(self, n):
        self.parent = list(range(n))

    def find(self, a):
        acopy = a
        while a != self.parent[a]:
            a = self.parent[a]
        while acopy != a:
            self.parent[acopy], acopy = a, self.parent[acopy]
        return a

    def union(self, a, b):
        self.parent[self.find(b)] = self.find(a)

n,_ = map(int,input().split())
a = [*map(int,input().split())]
pos = [*range(n)]

g = [[] for _ in range(n)]

UF = UnionFind(n)

for i in range(n):
    for j in range(i+1,n):
        if not (a[i]%a[j] == 0 or a[j]%a[i] == 0):
            continue
        if UF.find(i) != UF.find(j):
            g[i].append(j)  
            g[j].append(i)
            UF.union(i,j)

groups = [[] for _ in range(n)]
for i,x in enumerate(a):
    groups[UF.find(i)].append((x,i))

for row in groups:
    row.sort(reverse=1)

b = a[:]
for i in range(n):
    b[i] = groups[UF.find(i)].pop()

if [x for x,y in b] == sorted(a):
    print("JA")
else:
    print("NEJ")
    exit()


e = [y for x,y in b]
d = [0]*n
for i,y in enumerate(e):
    d[y] = i

active = [1]*n
degs = [len(x) for x in g]

moves = []
par = [-1]*n
for _ in range(n):
    # Find a leaf
    for i in range(n):
        if degs[i] == 1 and active[i]:
            break
    else:
        break

    if pos[i] == d[i]:
        active[i] = 0
        for nei in g[i]:
            degs[nei] -= 1
        continue

    # Find an active node that has value b[i]
    BFS = [i]
    par[i] = i
    vis = set()
    vis.add(i)
    for cur in BFS:
        for nei in g[cur]:
            if nei in vis or active[nei] == 0:
                continue
            vis.add(nei)
            BFS.append(nei)
            par[nei] = (cur)

            if pos[nei] == d[i]:
                break
        else:
            continue

        break

    assert(pos[BFS[-1]] == d[i])

    cur = BFS[-1]
    while par[cur] != cur:
        moves.append((pos[par[cur]],pos[cur]))
        pos[par[cur]],pos[cur] = pos[cur], pos[par[cur]]
        cur = par[cur]

    assert(pos[i] == d[i])

    active[i] = 0
    for nei in g[i]:
        degs[nei] -= 1
                
print(len(moves))
for x,y in moves:
    print(x+1,y+1)
