#!/usr/bin/python3
from collections import Counter

n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

occs = Counter(weights)

num_trips = occs[40]
if k >= 40:
    num_trips += (occs[20]+1)//2
else:
    num_trips += occs[20]
print(2*n+2*(coords[0] * num_trips))
