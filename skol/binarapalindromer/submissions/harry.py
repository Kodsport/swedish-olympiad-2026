#!/usr/bin/python3

a,b = map(int,input().split())

memo = {}

def calc(a):

    #räkna antal tal <= a som är palindrom under i bas 2
    ans = 0
    started = 0
    for i in range(65,-1,-1):

        if started:
            ans += dp(i-1,1,0,0)
            continue
        if (1<<i) + (i != 0) <= a:
            ans += dp(i-1,1,1,a-((1<<i) + (i != 0)))
            started = 1
        
    return ans

def dp(i1,i2,maxxed,a): #given that the number has already started (nonzero), number of ways to fill in the indicies [i1,i2]
    if i1 < i2:
        return 1
    if i1 == i2:
        if maxxed:
            return 1 + ((1<<i1) >= a)
        else:
            return 2
    
    if (i1,i2,maxxed,a) in memo:
        return memo[(i1,i2,maxxed,a)]

    ret = 0
    if not maxxed:
        ret += dp(i1-1,i2+1,0,0) + dp(i1-1,i2+1,0,0) #put 1 or 0
    else:
        if (1<<i1)+(1<<i2) > a:
            ret += dp(i1-1,i2+1,1,a)
        elif (1<<i1)+(1<<i2) <= a:
            ret += dp(i1-1,i2+1,0,0) + dp(i1-1,i2+1,1,a-((1<<i1)+(1<<i2)))

    memo[(i1,i2,maxxed,a)] = ret
    return ret



#print(calc(808661081088127437-1))
print(calc(b)-calc(a-1))
#print(memo)
# what 1 1024
# 1 1025
