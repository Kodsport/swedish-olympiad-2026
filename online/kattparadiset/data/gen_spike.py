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

n = int(cmdlinearg('n', 2e5))
k = int(cmdlinearg('k', max(2,n//2)))

a = int(cmdlinearg('a', 1))
b = int(cmdlinearg('b', 1))

# Distribute randomly
c = [1]*k

c[0] = a
c[1] = b

for _ in range(n-sum(c)):
    c[random.randint(1 + (b != 1),k-1)] += 1

random.shuffle(c)

alfa = "qwertyuiopasdlkjfhgzxcmnbv"

names = set()
while len(names) < k:
    names.add("".join([random.choice(alfa) for _ in range(random.randint(1,6))]))

names = list(names)
random.shuffle(names)



print(n,k)
for i in range(k):
    print(names[i],c[i])
