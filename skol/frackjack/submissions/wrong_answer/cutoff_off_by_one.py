#!/usr/bin/python3
# Otherwise correct, but puts the cutoff one metre too early: a throw of exactly
# 21 metres is counted as overshooting the post.  Dies already in group 1.
a = int(input())
b = int(input())

da = 21 - a if a < 21 else 10**9
db = 21 - b if b < 21 else 10**9

if da == db:
    print("Jack")
elif da < db:
    print("Alice")
else:
    print("Bertil")
