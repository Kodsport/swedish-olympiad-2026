
n = int(input())
a = [*map(int,input().split())]

if min(a) == max(a):
    print(0)
    exit()

s = sum(a)

maxelement = max(a)

factors = []
for d in range(1,n+1):
    if s%d != 0:
        continue
    fact = s//d
    if fact >= maxelement:
        factors.append(fact)

ans = n-1

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

smallest = 1e18
bad = set()

for d in factors:
    if d > smallest:
        continue

    founddivisor = 0
    for y in bad:
        if y%d == 0:
            founddivisor = 1
            break

    if founddivisor:
        continue

    UF = UnionFind(n)

    found = 0

    j = 0
    cur = a[0]
    for i in range(n):
        while cur < d:
            j = (j+1)%n
            cur += a[j]
        if cur == d:
            if UF.find(i) == UF.find((j+1)%n):
                found = 1
            UF.union(i,(j+1)%n)
        cur -= a[i]
    
    if found:
        ans = min(ans,n-s//d)
        smallest = min(smallest,d)
    else:
        bad.add(d)

print(ans)

