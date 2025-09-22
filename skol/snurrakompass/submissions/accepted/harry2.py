#!/usr/bin/python3

vis = [0]*360
vis[0] = 1
a,b = map(int,input().split())
BFS = [0]
for cur in BFS:
  for dx in [a,b,-a,-b]:
    nei = cur+dx
    nei %= 360
    if vis[nei]: continue
    vis[nei] = 1
    BFS.append(nei)

print(sum(vis))
