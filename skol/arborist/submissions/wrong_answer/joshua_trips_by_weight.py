#!/usr/bin/python3
# Derives the number of trips from the total weight, ceil(sum(k_i) / K), and
# then charges the trips to the farthest saplings.  Undercounts whenever K is
# not a multiple of the sapling weights, e.g. five 20 kg saplings with K = 50.

n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

trips = -(-sum(weights) // k)
print(2*n + 2*sum(sorted(coords, reverse=True)[:trips]))
