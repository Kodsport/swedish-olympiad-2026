n,a,b = map(int,input().split())
s = input()


best = n*a

for mask in range(1<<n):
    rest = [s[i] for i in range(n) if (mask>>i)&1]
    #print(rest)

    temp1 = (n-len(rest))*a

    m = len(rest)

    rest = rest*2
    
    
    for i in range(m):
        possible = 1
        stack = []
        for j in range(i,i+m):
            x = rest[j]
            if x == "(":
                stack.append(1)
            else:
                if not stack:
                    possible = 0
                    break
                else:
                    stack.pop()
        
        if possible and len(stack) == 0:
            best = min(best, temp1 + i*b)


print(best)
