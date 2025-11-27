#!/usr/bin/python3

N, Q = map(int,input().split())

room = set()

for _ in range(Q):
    a = [*map(int,input().split())]
    if a[0] == 1:
        if a[1] in room:
            print("Upptagen")
        else:
            room.add(a[1])
            print("Ledig")


