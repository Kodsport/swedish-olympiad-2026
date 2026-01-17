# https://github.com/cheran-senthil/PyRival/blob/master/pyrival/algebra/factors.py

from collections import Counter

def gcd(x, y):
    """greatest common divisor of x and y"""
    while y:
        x, y = y, x % y
    return x


def memodict(f):
    """memoization decorator for a function taking a single argument"""
    class memodict(dict):
        def __missing__(self, key):
            ret = self[key] = f(key)
            return ret

    return memodict().__getitem__


def pollard_rho(n):
    """returns a random factor of n"""
    if n & 1 == 0:
        return 2
    if n % 3 == 0:
        return 3

    s = ((n - 1) & (1 - n)).bit_length() - 1
    d = n >> s
    for a in [2, 325, 9375, 28178, 450775, 9780504, 1795265022]:
        p = pow(a, d, n)
        if p == 1 or p == n - 1 or a % n == 0:
            continue
        for _ in range(s):
            prev = p
            p = (p * p) % n
            if p == 1:
                return gcd(prev - 1, n)
            if p == n - 1:
                break
        else:
            for i in range(2, n):
                x, y = i, (i * i + i) % n
                f = gcd(abs(x - y), n)
                while f == 1:
                    x, y = (x * x + i) % n, (y * y + i) % n
                    y = (y * y + i) % n
                    f = gcd(abs(x - y), n)
                if f != n:
                    return f
    return n


@memodict
def prime_factors(n):
    """returns a Counter of the prime factorization of n"""
    if n <= 1:
        return Counter()
    f = pollard_rho(n)
    return Counter([n]) if f == n else prime_factors(f) + prime_factors(n // f)


def distinct_factors(n):
    """returns a list of all distinct factors of n"""
    factors = [1]
    for p, exp in prime_factors(n).items():
        factors += [p**i * factor for factor in factors for i in range(1, exp + 1)]
    return factors

n = int(input())
a = [*map(int,input().split())]

s = sum(a)

maxelement = max(a)

factors = distinct_factors(s)

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

for d in factors:
    #nxt = [-1]*n
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
            #nxt[i] = (j+1)%n
        cur -= a[i]
    
    if found:
        ans = min(ans,n-s//d)

print(ans)

