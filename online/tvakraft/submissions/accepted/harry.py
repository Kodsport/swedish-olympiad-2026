#!/usr/bin/python3

N = int(input())

for k in range(100):
    if N == (1 << k):
        print("Yes")
        exit()
        
print("No")
