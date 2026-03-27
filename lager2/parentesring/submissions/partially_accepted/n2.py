n,a,b = map(int,input().split())
s = input()*2


best = 1e18
for i in range(n):
    temp = i*b
    stack = []
    for j in range(i,i+n):
        x = s[j]
        if x == "(":
            stack.append(1)
        else:
            if not stack:
                temp += a
            else:
                stack.pop()

    temp += a*len(stack)

    best = min(best,temp)

print(best)
