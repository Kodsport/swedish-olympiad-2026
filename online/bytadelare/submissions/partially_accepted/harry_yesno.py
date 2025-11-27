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
g = [[] for _ in range(n)]

UF = UnionFind(n)

for i in range(n):
    for j in range(i+1,n):
        #print(i,j,len(a),n)
        if not (a[i]%a[j] == 0 or a[j]%a[i] == 0):
            continue
        if UF.find(i) != UF.find(j):
            g[i].append(j)  
            g[j].append(i)
            UF.union(i,j)

groups = [[] for _ in range(n)]
for i,x in enumerate(a):
    groups[UF.find(i)].append(x)

for row in groups:
    row.sort(reverse=1)

b = a[:]
for i in range(n):
    b[i] = groups[UF.find(i)].pop()

if b == sorted(a):
    print("JA")
else:
    print("NEJ")



print(0)
