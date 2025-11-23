#!/usr/bin/python3

a,b = map(int,input().split())

memo = {}
# 64*2*2*a
def dp(i1,i2,maxxed,a,nonzero,started):
    if (i1,i2,maxxed,a,nonzero,started) in memo:
        return memo[(i1,i2,maxxed,a,nonzero,started)]
    
    if i2 > i1:
        return nonzero
    ret = 0
    

    if i1 == i2:
        return nonzero + (not (((a>>i1)&1 == 0) and maxxed))
    
    if (1<<i1) > a:
        memo[(i1,i2,maxxed,a,nonzero,started)] = dp(i1-1,i2,1,a,0,started)
        return memo[(i1,i2,maxxed,a,nonzero,started)]
    
    print(i1,i2,maxxed,a)

    if i2 == 0 and not started:
        ret = dp(i1-1,0,0,a,0,0)
    
    


    if maxxed:
        mask = (1<<(i1-i2+1))-1
        mask <<= i2
        mask &= a
 
        #ind = (((a>>i1)&1)) * (((a>>i2)&1)) # 1 if we can set index i1 and i2 to 1, otherwise 0
        ind = ((1<<i1) + (1<<i2)) <= mask
        ind2 = (((a>>i1)&1) == 0) * (((a>>i2)&1) == 0)

        print(ind,ind2)
        if ind2:
            ret += dp(i1-1,i2+1,1,a,nonzero,started)*started
        else:
            ret += dp(i1-1,i2+1,0,a,nonzero,started)*started + ind*dp(i1-1,i2+1,1,a,1,1)
    else:
        ret += dp(i1-1,i2+1,0,a,nonzero,started)*started + dp(i1-1,i2+1,0,a,1,1)
    
    memo[(i1,i2,maxxed,a,nonzero,started)] = ret

    return ret
print(dp(64,0,1,b,0,0))

# 0 1 10 11 100 101 110
print(dp(64,0,1,b,0,0)-dp(64,0,1,a-1,0,0) )
