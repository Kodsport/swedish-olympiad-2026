#!/usr/bin/python3

def f(r,c):
    return F[c]-r

def horisontal(c1,c2,r):
    if c1 > c2:
        (c1,c2) = (c2,c1)
    res = 0
    while c1 <= c2:
        res += f(r,c1)
        c1 += 1
    return res

def vertical(c,r1,r2):
    if r1 > r2:
        (r1,r2) = (r2,r1)
    d = r2-r1+1
    return d*F[c] - d*r1 - (d**2-d)//2


R,C = map(int,input().split())
F = list(map(int, input().split()))
sx,sy = map(int,input().split())
gx,gy = map(int,input().split())
if sy < gy:
    (sx,sy,gx,gy) = (gx,gy,sx,sy)
# sy >= gy

ans = 10**20

# via one column
for i in range(C):
    temp = vertical(i,sy,gy) + horisontal(sx,i,sy) + horisontal(gx,i,gy) - f(sy,i) - f(gy,i)
    ans = min(ans, temp)

# via two columns and r = R-1
for c1 in range(C):
    for c2 in range(C):
        temp = horisontal(sx,c1,sy) + vertical(c1,sy,R-1) + horisontal(c1,c2,R-1) + vertical(c2,R-1,gy) + horisontal(c2,gx,gy)
        temp -= f(sy,c1) + f(R-1,c1) + f(R-1,c2) + f(gy,c2)
        ans = min(ans, temp)

print(ans)


