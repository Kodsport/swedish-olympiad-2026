#!/usr/bin/python3


n = int(input())
a = [*map(int,input().split())]

arrs = [[a[0]]]

for x in a[1:]:
    lo = 0
    hi = len(arrs)-1

    while lo < hi:
        mid = (lo + hi)//2
        if arrs[mid][-1] < x:
            hi = mid
        else:
            lo = mid + 1

    if arrs[lo][-1] >= x:
        arrs.append([x])
    else:
        arrs[lo].append(x)


for row in arrs:
    print(*row)
