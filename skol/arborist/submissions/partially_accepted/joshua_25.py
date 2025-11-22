#!/usr/bin/python3
n,k=map(int,input().split())

coords = list(map(int,input().split()))
weights = list(map(int,input().split()))

print(2*n+2*sum(coords))
