#!/usr/bin/python3
# @EXPECTED_GRADES@ AC TLE TLE

from itertools import combinations

a,b,k = map(int,input().split())

out = 0
for mask in range(1,1<<(len(bin(b))-2)):
    temp = bin(mask)[2:]
    temp = int(temp + temp[::-1],2)

    out += (a <= temp <= b and bin(temp).count("1") == k)

    if temp > b:
        break

for mask in range(1,1<<(len(bin(b))-2)):
    temp = bin(mask)[2:]
    
    temp = int(temp[:-1] + temp[::-1],2)
    
    out += (a <= temp <= b and bin(temp).count("1") == k)

    if temp > b:
        break

print(out)


