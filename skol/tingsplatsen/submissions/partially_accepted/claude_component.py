#!/usr/bin/python3
# Correct predicate, but assumes the town squares form one connected region and
# flood fills outwards from the first one found.
# Right for N=1 and for the rectangle group (both have a unique answer), wrong
# in general -- the answer set can be several components, e.g. 10 isolated cells.

n, m = map(int, input().split())
grid = [list(input()) for _ in range(n)]

farms = [(i, j) for i in range(n) for j in range(m) if grid[i][j] == '*']

ok = set()
for i in range(n):
    for j in range(m):
        if grid[i][j] == '*':
            continue
        if len({abs(i - a) + abs(j - b) for a, b in farms}) == 1:
            ok.add((i, j))

start = min(ok)
stack = [start]
seen = {start}
while stack:
    i, j = stack.pop()
    for di, dj in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        p = (i + di, j + dj)
        if p in ok and p not in seen:
            seen.add(p)
            stack.append(p)

for i, j in seen:
    grid[i][j] = 'X'

for row in grid:
    print("".join(row))
