#!/usr/bin/python3

a,b,k = map(int,input().split())

out = 0
for x in range(a,b+1):
    if bin(x)[2:] == bin(x)[2:][::-1] and bin(x).count("1") == k:
        out += 1

print(out)


