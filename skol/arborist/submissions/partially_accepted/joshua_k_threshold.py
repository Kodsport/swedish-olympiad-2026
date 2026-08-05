#!/usr/bin/python3
# Thinks a sapling weighs "half a load" as soon as K/20 >= 1.5, i.e. pairs
# saplings already from K = 30 instead of from K = 40.
# Killed by the K in [30,39] test cases; only group 1 (K = 25) survives.

n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

small = sorted((coords[i] for i in range(n) if weights[i] == 20), reverse=True)
big = [coords[i] for i in range(n) if weights[i] == 40]

step = 2 if k / 20 >= 1.5 else 1
print(2*n + 2*sum(big) + 2*sum(small[::step]))
