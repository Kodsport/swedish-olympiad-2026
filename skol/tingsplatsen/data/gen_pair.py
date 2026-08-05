#!/usr/bin/python3
# Places exactly two farms at given coordinates in an n x m grid.
#
# With two farms at (r1,c1), (r2,c2) the set of town squares is
# {(i,j) : |i-r1|+|j-c1| = |i-r2|+|j-c2|}, which lets us dial in shapes that
# ordinary "find the one answer" solutions get wrong:
#   * n=1  -> the unique midpoint, used to probe the N=1 group at its extremes
#   * a knight-free diagonal pair -> the answer set is a large disconnected blob
#   * an anti-diagonal pair       -> the answer set is 10 isolated single cells

import sys


def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default


n = int(cmdlinearg('n'))
m = int(cmdlinearg('m'))
r1 = int(cmdlinearg('r1'))
c1 = int(cmdlinearg('c1'))
r2 = int(cmdlinearg('r2'))
c2 = int(cmdlinearg('c2'))

grid = [['.'] * m for _ in range(n)]
for i, j in ((r1, c1), (r2, c2)):
    assert grid[i][j] == '.', "the two farms coincide"
    grid[i][j] = '*'

print(n, m)
for row in grid:
    print("".join(row))
