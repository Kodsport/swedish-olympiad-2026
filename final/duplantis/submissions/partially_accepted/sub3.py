#!/usr/bin/python3

n,v = map(int,input().split())

A = list(map(int,input().split()))

if v >= max(A):
    print(0)
elif v < A[0] and v < A[1]-1:
    print(2)
    print(max(A), A[1])
else:
    print(1)
    print(max(A))
