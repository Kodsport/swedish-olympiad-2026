#!/usr/bin/python3

import sys
import random
import math

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
m = int(cmdlinearg("m"))
p = int(cmdlinearg("p", 2))

R = random.randrange(n)
C = random.randrange(m)

points_by_distance = [[] for _ in range(n+m)]

for i in range(n):
    for j in range(m):
        d = abs(i-R) + abs(j-C)
        points_by_distance[d].append((i,j))

order = list(range(n+m))
random.shuffle(order)

d_best = -1
score = 10**10

for d in order:
    s2 = max(0, p-len(points_by_distance[d]))
    if s2 < score:
        score = s2
        d_best = d

p = min(p, len(points_by_distance[d_best]))
houses = random.sample(points_by_distance[d_best], p)

grid = [['.']*m for _ in range(n)]
for i,j in houses:
    grid[i][j] = '*'

print(n,m)
for i in range(n):
    print("".join(grid[i]))
