#!/usr/bin/python3

n = int(input())
a = [input() for _ in range(n)]


# Search min(x1,x2)
for lo in range(n):
    b = [row[:] for row in a]
    for i in range(lo+1):
        b[i] = b[i].replace(".","#")
    
    print("?")
    for row in b:
        print(row)

    ret = int(input())
    if ret == -1:
        x1 = lo-1
        break

# Search max(x1,x2)
for hi in range(n-1,-1,-1):
    b = [row[:] for row in a]
    for i in range(n-1,hi-1,-1):
        b[i] = b[i].replace(".","#")
    
    print("?")
    for row in b:
        print(row)

    ret = int(input())
    if ret == -1:
        x2 = hi+1
        break

# Search min(y1,y2)
for lo in range(n):
    b = [row[:] for row in a]

    for i in range(n):
        b[i] = b[i][:lo+1].replace(".","#") + b[i][lo+1:]
    
    print("?")
    for row in b:
        print(row)

    ret = int(input())
    if ret == -1:
        y1 = lo-1
        break

# Search max(y1,y2)
for hi in range(n-1,-1,-1):
    b = [row[:] for row in a]
    for i in range(n):
        b[i] = b[i][:hi] + b[i][hi:].replace(".","#")
    
    print("?")
    for row in b:
        print(row)

    ret = int(input())
    if ret == -1:
        y2 = hi+1
        break


# antingen  (x1,y1) (x2,y2)
# eller     (x1,y2) (x2,y1)

x1,x2 = min(x1,x2), max(x1,x2)
y1,y2 = min(y1,y2), max(y1,y2)

b = [row[:] for row in a]
for x in range(x1+2):
    b[x] = b[x][:y1+1] + "#" + b[x][y1+2:]

b[x1+1] = b[x1+1][:y1+2].replace(".","#") + b[x1+1][y1+2:]
print("?")
for row in b:
    print(row)

ret = int(input())

if (ret == -1):
    print("!",x1,y1,x2,y2)
else:
    print("!",x1,y2,x2,y1)
