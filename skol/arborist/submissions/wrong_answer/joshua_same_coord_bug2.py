#!/usr/bin/python3

# does not ceil div

from collections import Counter

n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

occs = Counter(weights)

num_trips = occs[40]+occs[20]//2
print(2*n+2*(coords[0] * num_trips))
