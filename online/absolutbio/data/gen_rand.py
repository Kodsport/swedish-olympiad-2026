#!/usr/bin/python3

import sys
import random
from random import randint

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))

N = int(cmdlinearg('N', random.randint(1, int(cmdlinearg("maxn", 10**9)))))
Q = int(cmdlinearg('Q', random.randint(1, 2*10**5)))


mode = cmdlinearg('mode', 'random')

fillup = int(cmdlinearg('fillup', Q//2))

sample = set()
while len(sample) < fillup:
    sample.add(random.randint(1, N))

sample = list(sample)
random.shuffle(sample)

queries = [(1,x) for x in sample]

used = set(sample)

p = float(cmdlinearg('p', 0))
p2 = int(cmdlinearg('p2', 2))

while len(queries) < Q:
    type = random.randint(1, 2)
    if type == 1 or mode == "onlyadd":
        x = random.randint(1, N)
        #print(x,p)
        if random.random() <= p:
            if len(used) < 100:
                x = random.choice(list(used))
            else:
                k = random.randint(0, min(100,N - len(used)))
                for y in used:
                    x = y
                    if k == 0:
                        break
                    k -= 1

        queries.append((1,x))
        used.add(x)
    else:

        k = random.randint(0, min(10,len(used)-1))
        for x in used:
            l = x
            if k == 0:
                break
            k -= 1

        r = random.randint(l, N)

        if random.randint(1,p2) == 2:
            k = random.randint(0, min(10,N - len(used)))
            for x in used:
                r = x
                if k == 0:
                    break
                k -= 1

        while l == r:
            r = random.randint(1, N)

        queries.append((2,l,r))
        if r in used:
            continue
        used.remove(l)
        used.add(r)


print(N,Q)

for q in queries:
    print(*q)

