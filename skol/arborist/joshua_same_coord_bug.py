#!/usr/bin/python3
# We choose not to kill this in the skolkval testdata:
# it's more likely that someone makes the mistake of incorrectly
# handling k=40 than k=20

from collections import Counter

n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

occs = Counter(weights)

num_trips = occs[40]+(occs[20]+1)//2
print(2*n+2*(coords[0] * num_trips))
