#!/usr/bin/python3
# Chooeses best coordinate correctly, but then takes any index with that coordinate

n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

time = 2*n

while coords:
    curr_weight = 0
    max_coord = -1
    while 1:
        best_val = max((coords[i] for i in range(len(coords)) if weights[i]+curr_weight <= k), default=-1)
        if best_val == -1:
            break
        tree_ind = coords.index(best_val)
        max_coord = max(max_coord, coords[tree_ind])
        curr_weight += weights[tree_ind]
        coords.pop(tree_ind)
        weights.pop(tree_ind)
    assert max_coord != -1
    time += 2*max_coord

print(time)
