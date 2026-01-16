#!/usr/bin/python3

n = int(input())
W = list(map(int,input().split()))

seen = set()
p = -1

for i in range(n):
    if W[i] in seen:
        if p == -1 or W[p] < W[i]:
            p = i
    seen.add(W[i])

q = -1
for i in range(n):
    if W[i] == W[p]:
        q = i
        break

op = []
for i in range(n):
    if i != p:
        op.append("p")
    else:
        op.append("+ "+str(i+1))
ops = [op]

wq = W[q]
if 0 < W[q] < 128:
    wq -= 1

for b in range(8):
    op = []
    for i in range(n):

        if i != p and i != q:
            if ((1 << b)&W[i])!=0:
                op.append("| "+str(p+1))
            else:
                op.append("p")
        else:
            if b < 7:
                if i == p:
                    op.append("< "+str(p+1))
                else:
                    if ((1 << b)&wq)!=0:
                        op.append("| "+str(p+1))
                    else:
                        op.append("p")
            else:
                if i == p:
                    if W[q] >= 128:
                        op.append("| "+str(q+1))
                    elif W[q] == 0:
                        op.append("& "+str(q+1))
                    else:
                        op.append("+ "+str(q+1))
                else:
                    if W[q] >= 128:
                        op.append("| "+str(p+1))
                    elif W[q] == 0:
                        op.append("p")
                    else:
                        op.append("+ "+str(q+1))
    ops.append(op)

print(len(ops))

for op in ops:
    for s in op:
        print(s)
