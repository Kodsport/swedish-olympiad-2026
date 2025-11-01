#!/usr/bin/python3
#@EXPECTED_GRADES@ AC WA WA AC WA WA

n,k = map(int,input().split())

out = []
for _ in range(k):
    color, amount = input().split()
    out += [color]*int(amount)

import random

def check():
    for i in range(n):
        if out[i] == out[i-1]:
            return 0
    return 1

for _ in range(round(2e6//n + 1)):
    if check():
        print("Ja")
        print(*out)
        exit()
    random.shuffle(out)

print("Nej")

