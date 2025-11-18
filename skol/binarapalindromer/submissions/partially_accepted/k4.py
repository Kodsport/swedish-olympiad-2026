#!/usr/bin/python3
from itertools import combinations

a,b,k = map(int,input().split())

out = 0
for comb in combinations([*range(0,30)],k//2):
    num = [0]*30
    for x in comb:
        num[x] = 1
    num.reverse()
    while num[-1] == 0:
        num.pop()
    
    if k%2:
        temp = num[::-1]+[1]+num
    else:
        temp = num[::-1]+num
    
    temp = sum(1<<i for i in range(len(temp)) if temp[i])

    out += a <= temp <= b

print(out)


