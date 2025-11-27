#!/usr/bin/python3

N, Q = map(int,input().split())

room = [0]*(N+5)

for _ in range(Q):
    a = [*map(int,input().split())]
    if a[0] == 1:
        if room[a[1]]:
            print("Upptagen")
        else:
            room[a[1]] = 1
            print("Ledig")
    else:
        if room[a[2]]:
            #room.remove(a[1])
            print("Upptagen")
        else:
            print("Ledig")
            room[a[2]] = 1
            room[a[1]] = 0
