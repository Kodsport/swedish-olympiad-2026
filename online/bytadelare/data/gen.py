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

def valid_shuffle(A, rev=False):
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
                    for i2 in range(0, len(A)):
                        if seen[i2] == 0 and i2 != a and (A[i2]%A[a] == 0 or A[a]%A[i2] == 0):
                            Q.append(i2)
                j += 1
            comp2 = component.copy()
            if rev:
                comp2 = comp2[::-1]
            else:
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
noise = int(cmdlinearg('noise', 0))

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

elif mode == "prime_forest":
    comps = int(cmdlinearg("comps", 1))
    branch = int(cmdlinearg("branch", n))
    p_choose = (n+comps) // 2
    p_choose = min(p_choose, len(small_primes))
    primes = random.sample(small_primes, p_choose)

    for i in range(len(primes)):
        A.append(primes[i])
        if i >= comps:
            start = max(0, i-branch)
            par = random.randrange(start, i)
            A.append(primes[par]*primes[i])
    
    while len(A) < n:
        a = random.randrange(len(A))
        A.append(A[a])
    
    A = valid_shuffle(A)

elif mode == "prime_line":
    primes = []
    p = 0
    num = maxx
    while True:
        if p >= len(small_primes):
            break
        while num > 1 and ((not is_prime(num)) or num*small_primes[p] > maxx):
            num -= 1
        if num <= small_primes[p]:
            break
        primes.append(num)
        primes.append(small_primes[p])
        p += 1
        num -= 1

    for i in range(len(primes)-1):
        A.append(primes[i])
        A.append(primes[i]*primes[i+1])
    A.append(primes[-1])

    if len(A) > n:
        A = A[::n]

    
    for i in range(n-len(A)):
        if i%2 == 0:
            A.append(primes[0])
        else:
            A.append(primes[-1])
    

    A = valid_shuffle(A, True)       

elif mode == "dense":
    A = [small_primes[0]] * ((n-1) // 2)
    A += [small_primes[1]] * ((n-1) // 2)
    A.append(small_primes[0]*small_primes[1])
    A = valid_shuffle(A, True) 
        


elif mode == "one_edge":
    assert u >= 2
    diff[0] = random.randint(2, maxx // 2)
    diff[1] = 2*diff[0]
    if u >= 3:
        diff[2] = random.randint(2,maxx)
    for _ in range(n):
        A.append(diff[random.randrange(len(diff))])
    A = valid_shuffle(A)

elif mode == "two_edge":
    assert u >= 3
    diff[0] = random.randint(2, maxx // 3)
    diff[1] = 2*diff[0]
    diff[2] = 3*diff[0]
    for _ in range(n):
        A.append(diff[random.randrange(len(diff))])

if a1 != -1:
    A[0] = a1


for _ in range(noise):
    i,j = random.randrange(len(A)), random.randrange(len(A))
    A[i],A[j] = A[j],A[i] 

print(len(A),x)
print(*A)



