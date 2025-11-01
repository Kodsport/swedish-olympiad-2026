#!/usr/bin/python3
# @EXPECTED_GRADES@ AC AC AC WA WA WA

n,k = map(int,input().split())

out = [""]*n

inds = [*range(0,n,2)] + [*range(1,n,2)]

for _ in range(k):
    color, amount = input().split()
    amount = int(amount)
    while amount:
        amount -= 1
        out[inds.pop()] = color


def check():
    for i in range(n):
        if out[i] == out[i-1]:
            return 0
    return 1

if check():
    print("Ja")
    print(*out)
    exit()

print("Nej")

