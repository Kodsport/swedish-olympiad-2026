#!/usr/bin/python3
import math

def binomial(a,b):
    if a < b or a < 0 or b < 0:
        return 0
    return math.factorial(a) // (math.factorial(a-b) * math.factorial(b))

def solve(x):
    if x == 0:
        return 0
    str_x = str(bin(x))[2:]
    n = len(str_x)
    half = str_x[:n//2]
    
    extra = 0
    mid = '0'
    if k%2 == 1:
        mid = '1'
    h1 = half+half[::-1]
    h2 = half+mid+half[::-1]
    if n%2 == 0 and h1 <= str_x and h1.count('1') == k:
        extra = 1
    if n%2 == 1 and h2 <= str_x and h2.count('1') == k:
        extra = 1
    goal = k//2

    ans = 0
    have = 0
    tot = len(half)
    for c in half:
        tot -= 1
        if c == '1':
            if tot != len(half)-1:
                ans += binomial(tot, goal-have)
            have += 1
    if k%2 == 1 and n%2 == 0:
        ans = 0
    return ans + extra + solve(2**(n-1)-1)
    

l,r,k = map(int,input().split())
print(solve(r)-solve(l-1))
