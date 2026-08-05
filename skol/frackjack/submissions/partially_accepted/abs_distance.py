#!/usr/bin/python3
# Measures the distance to the post as |21 - x|, i.e. treats overshooting as
# just as good as falling short.  Correct as long as nobody overshoots, so it
# takes group 1 and 2 and dies on e.g. (22, 20), which it calls a draw.
a = int(input())
b = int(input())

da = abs(21 - a)
db = abs(21 - b)

if da == db:
    print("Jack")
elif da < db:
    print("Alice")
else:
    print("Bertil")
