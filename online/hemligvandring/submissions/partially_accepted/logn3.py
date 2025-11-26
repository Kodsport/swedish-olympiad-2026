#!/usr/bin/python3

n = int(input())
a = [input() for _ in range(n)]


# Search min(x1,x2)

lo = 0
hi = n-1
while lo < hi:
    mid = (lo+hi)//2

    ind = mid

    b = [row[:] for row in a]
    for i in range(ind+1):
        b[i] = b[i].replace(".","#")
    
    print("?")
    for row in b:
        print(row)

    ret = int(input())
    if ret != -1:
        lo = mid+1
    else:
        hi = mid

x1 = lo-1

# Search max(x1,x2)
lo = 0
hi = (n-1)
while lo < hi:
    mid = (lo+hi + 1)//2

    ind = mid

    b = [row[:] for row in a]
    for i in range(n-1,ind-1,-1):
        b[i] = b[i].replace(".","#")
    
    print("?")
    for row in b:
        print(row)

    ret = int(input())
    if ret != -1:
        hi = mid-1
    else:
        lo = mid

x2 = lo+1

# Search min(y1,y2)
lo = 0
hi = (n-1)
while lo < hi:
    mid = (lo+hi)//2

    ind = mid

    b = [row[:] for row in a]
    for i in range(n):
        b[i] = b[i][:ind+1].replace(".","#") + b[i][ind+1:]
    
    print("?")
    for row in b:
        print(row)

    ret = int(input())
    if ret != -1:
        lo = mid+1
    else:
        hi = mid

y1 = lo-1


lo = 0
hi = (n-1)
while lo < hi:
    mid = (lo+hi + 1)//2

    ind = mid

    b = [row[:] for row in a]
    for i in range(n):
        b[i] = b[i][:ind] + b[i][ind:].replace(".","#")
    
    print("?")
    for row in b:
        print(row)

    ret = int(input())
    if ret != -1:
        hi = mid-1
    else:
        lo = mid

y2 = lo+1


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
