#!/usr/bin/python3

a,b,k = map(int,input().split())

memo = {}

def calc(a):

    #räkna antal tal <= a som är palindrom under i bas 2
    ans = 0
    started = 0
    for i in range(65,-1,-1):

        if started:
            ans += dp(i-1,1,0,0,k-1-(i!=0))
            continue
        if (1<<i) + (i != 0) <= a:
            ans += dp(i-1,1,1,a-((1<<i) + (i != 0)),k-1-(i!=0))
            started = 1
        
    return ans

def dp(i1,i2,maxxed,a,k): #given that the number has already started (nonzero), number of ways to fill in the indicies [i1,i2]
    if k < 0:
        return 0
    
    if k == 0:
        return 1
    
    if i1 < i2:
        return 0
    
    if i1 == i2:
        if maxxed:
            return (k == 0) + ((1<<i1) <= a and k == 1)
        else:
            return (k == 0) + (k == 1)
    
    if (i1,i2,maxxed,a,k) in memo:
        return memo[(i1,i2,maxxed,a,k)]

    ret = 0
    if not maxxed:
        ret += dp(i1-1,i2+1,0,0,k) + dp(i1-1,i2+1,0,0,k-2) #put 1 or 0
    else:
        if (1<<i1)+(1<<i2) > a:
            ret += dp(i1-1,i2+1,1,a,k)
        elif (1<<i1)+(1<<i2) <= a:
            ret += dp(i1-1,i2+1,0,0,k) + dp(i1-1,i2+1,1,a-((1<<i1)+(1<<i2)),k-2)

    memo[(i1,i2,maxxed,a,k)] = ret
    return ret




print(calc(b)-calc(a))


