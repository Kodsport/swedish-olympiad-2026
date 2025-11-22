#!/usr/bin/python3
# Only considers the max coord. Can fail if we first take tree of weight 20, and then the
# next max coord tree is 40, and there is a 20-tree to the left

n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

time = 2*n

def argmax(lst):
    return lst.index(max(lst))

while coords:
    curr_weight = 0
    max_coord = -1
    while coords:
        tree_ind = argmax(coords)
        if curr_weight + weights[tree_ind] > k:
            break
        max_coord = max(max_coord, coords[tree_ind])
        curr_weight += weights[tree_ind]
        coords.pop(tree_ind)
        weights.pop(tree_ind)
    assert max_coord != -1
    time += 2*max_coord

print(time)
