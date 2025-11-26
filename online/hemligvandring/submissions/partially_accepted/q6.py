#!/usr/bin/python3

# 6 queries

n = int(input())
A = [input() for _ in range(n)]

print("?")
for row in A:
    print(row)

k = int(input())



B = [[*row] for row in A]
for i in range(n-1):
    for j in range(0,n,2):
        B[i][j] = "#"

print("?")
for row in B:
    print("".join(row))

a = int(input())


B = [[*row] for row in A]
for i in range(1,n):
    for j in range(0,n,2):
        B[i][j] = "#"

print("?")
for row in B:
    print("".join(row))

b = int(input())


B = [[*row] for row in A]
for i in range(0,n,2):
    for j in range(n-1):
        B[i][j] = "#"

print("?")
for row in B:
    print("".join(row))

c = int(input())

B = [[*row] for row in A]
for i in range(0,n,2):
    for j in range(1,n):
        B[i][j] = "#"

print("?")
for row in B:
    print("".join(row))

d = int(input())


x1 = n-1 - (a-k)//2
x2 = (b-k)//2
y1 = n-1 - (c-k)//2
y2 = (d-k)//2

# antingen  (x1,y1) (x2,y2)
# eller     (x1,y2) (x2,y1)

x1,x2 = min(x1,x2), max(x1,x2)
y1,y2 = min(y1,y2), max(y1,y2)

B = [row[:] for row in A]
for x in range(x1+2):
    B[x] = B[x][:y1+1] + "#" + B[x][y1+2:]

B[x1+1] = B[x1+1][:y1+2].replace(".","#") + B[x1+1][y1+2:]
print("?")
for row in B:
    print(row)

ret = int(input())

if (ret == -1):
    print("!",x1,y1,x2,y2)
else:
    print("!",x1,y2,x2,y1)