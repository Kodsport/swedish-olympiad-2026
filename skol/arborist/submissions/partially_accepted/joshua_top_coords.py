#!/usr/bin/python3
# Counts the trips correctly, but pays 2*x for the farthest x's instead of for
# the saplings that actually end up alone in a trip.  A 40 kg sapling close to
# the origin is charged as if it stood at the far end.

n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

c20 = weights.count(20)
c40 = weights.count(40)
trips = c40 + ((c20+1)//2 if k >= 40 else c20)
print(2*n + 2*sum(sorted(coords, reverse=True)[:trips]))
