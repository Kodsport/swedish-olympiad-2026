n,a,b = map(int,input().split())
s = input()*2

best = 1e18
for start in range(n):
    temp = start*b

    end = start+n

    memo = {}
    def dp(i,cnt):
        if i == end:
            return 0 if cnt == 0 else 1e18
        if (i,cnt) in memo:
            return memo[(i,cnt)]

        best = 1e18
        if s[i] == ")":
            # ta 
            if cnt:
                best = min(dp(i+1,cnt-1), best)
            
            # inte ta
            best = min(dp(i+1,cnt)+a,best)
        
        elif s[i] == "(":
            # ta 

            best = min(best, dp(i+1,cnt+1), dp(i+1,cnt)+a)
        
        memo[(i,cnt)] = best
        return best


    best = min(best,dp(start,0)+temp)

print(best)
