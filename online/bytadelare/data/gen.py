#!/usr/bin/python3

import sys
import random

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

def is_prime(x):
    for d in range(2, x):
        if x%d == 0:
            return False
        if d*d > x:
            return True
    return True

def valid_shuffle(A):
    A2 = []
    for i in range(len(A)):
        A2.append((A[i], i))
    A2.sort()
    goal = [-1] * len(A)
    res = [-1] * len(A)
    seen = [0] * len(A)
    for i in range(len(A)):
        goal[A2[i][1]] = i
    for i in range(len(A)):
        if seen[i] == 0:
            # BFS
            Q = [i]
            component = []
            j = 0
            while j < len(Q):
                a = Q[j]
                if seen[a] == 0:
                    seen[a] = 1
                    component.append(a)
                    for i2 in range(0, n):
                        if seen[i2] == 0 and i2 != a and (A[i2]%A[a] == 0 or A[a]%A[i2] == 0):
                            Q.append(i2)
                j += 1
            comp2 = component.copy()
            random.shuffle(comp2)
            for j in range(len(comp2)):
                res[goal[comp2[j]]] = A[component[j]]
    return res


random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n', 1))
x = int(cmdlinearg('x', 1))
maxx = int(cmdlinearg('maxx', 10**6))
a1 = int(cmdlinearg('a1', -1))
u = int(cmdlinearg('u', 1000))
mode = cmdlinearg('mode', 'random')

small_primes = []
for i in range(2, int(maxx**0.5)+1):
    if is_prime(i):
        small_primes.append(i)

prime_powers = []
for p in small_primes:
    p2 = p
    while p2 <= maxx:
        prime_powers.append(p2)
        p2 *= p

u = min(u, maxx)

A = []
diff = random.sample(range(1, maxx+1), u)

if mode == "random":
    for _ in range(n):
        A.append(diff[random.randrange(len(diff))])

elif mode == "prime_power":
    for _ in range(n):
        A.append(prime_powers[random.randrange(len(prime_powers))])
    A = valid_shuffle(A)

if a1 != -1:
    A[0] = a1

print(len(A),x)
print(*A)



