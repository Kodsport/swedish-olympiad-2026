#!/usr/bin/python3
from random import randint

r,c=map(int,input().split())
grid = [input().strip() for _ in range(r)]

def is_blocked(row, col):
    return grid[row%r][col%c] == '#'

print(r,c)
[print(row) for row in grid]

#max_coord = 10**9
max_coord = 500

# Same, possible
while True:
    row = randint(0, max_coord)
    col = randint(0, max_coord)
    if not is_blocked(row, col):
        print(col, row, col, row)
        break

# random, likely impossible
while True:
    row1 = randint(0, max_coord)
    col1 = randint(0, max_coord)
    row2 = randint(0, max_coord)
    col2 = randint(0, max_coord)
    if not is_blocked(row1, col1) and not is_blocked(row2, col2):
        print(col1, row1, col2, row2)
        break

# offset, impossible
while True:
    row1 = randint(0, max_coord)
    col1 = randint(0, max_coord)
    row2 = row1
    col2 = col1
    if randint(0,1) == 0:
        row2 += 2
    else:
        col2 += 2
    if not is_blocked(row1, col1) and not is_blocked(row2, col2):
        print(col1, row1, col2, row2)
        break

# two random possible
for _ in range(1):
    while True:
        row1 = randint(0, max_coord)
        col1 = randint(0, max_coord)
        d = randint(0, max_coord)
        row2 = row1 + d
        col2 = col1 + d
        if row2 > max_coord or col2 > max_coord or col2 < 0:
            continue

        if not is_blocked(row1, col1) and not is_blocked(row2, col2):
            print(col1, row1, col2, row2)
            break

while True:
    row = randint(0, max_coord)
    col = randint(0, max_coord)
    if not is_blocked(row, col):
        print(col, row, col, row)
        break

