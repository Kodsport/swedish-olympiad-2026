#!/usr/bin/python3
# Places four farms on the corners of the axis-aligned rectangle
# rows r1,r2 and columns c1,c2 of an n x m grid (the group-2 shape).
#
# The unique town square is the centre ((r1+r2)/2, (c1+c2)/2), so the parameters
# control the aspect ratio of the rectangle and how much slack there is between
# the rectangle and the edges of the grid -- the two things a solution that
# conflates "the grid" with "the bounding box of the farms" gets wrong.

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
r2 = int(cmdlinearg('r2'))
c1 = int(cmdlinearg('c1'))
c2 = int(cmdlinearg('c2'))

assert r1 < r2 and c1 < c2, "not a rectangle"

grid = [['.'] * m for _ in range(n)]
for i in (r1, r2):
    for j in (c1, c2):
        grid[i][j] = '*'

print(n, m)
for row in grid:
    print("".join(row))
