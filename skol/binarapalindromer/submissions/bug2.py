#!/usr/bin/python3

a,b = map(int,input().split())

memo = {}

def dp(i1,i2,maxxed,a): #given that the number has already started (nonzero), number of ways to fill in the indicies [i1,i2]
    if i1 < i2:
        return 1
    if i1 == i2:
        if maxxed:
            return 1 + ((1<<i1)&a > 0)
        else:
            return 2
    
    if (i1,i2,maxxed,a) in memo:
        return memo[(i1,i2,maxxed,a)]

    ret = 0
    if not maxxed:
        ret += dp(i1-1,i2+1,0,a) + dp(i1-1,i2+1,0,a-((1<<i1)-(1<<i2))) #put 1 or 0
    else:
        if ((a>>i1)&1) == 0 and ((a>>i2)&1) == 0:
            ret += dp(i1-1,i2+1,1,a)
        elif ((a>>i1)&1) and ((a>>i2)&1):
            ret += dp(i1-1,i2+1,0,a) + dp(i1-1,i2+1,1,a-((1<<i1)-(1<<i2)))
        else:
            ret += dp(i1-1,i2+1,0,a)

    memo[(i1,i2,maxxed,a)] = ret
    return ret

def calc(a):
    #räkna antal tal <= a som är palindrom under i bas 2
    ans = 0
    started = 0
    for i in range(65,-1,-1):
        if started:
            ans += dp(i-1,1,0,a-((1<<i) + (i != 0)))
            continue
        if (1<<i) + (i != 0) <= a:
            ans += dp(i-1,1,1,a-((1<<i) + (i != 0)))
        if a&(1<<i):
            started = 1
        
    return ans

print(calc(b)-calc(a-1))
