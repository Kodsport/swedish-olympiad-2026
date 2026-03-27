# @EXPECTED_GRADES@ AC WA WA WA WA WA WA WA
n,a,b = map(int,input().split())
s = [*input()]

temp = 0
stack = []
for j in range(n):
    x = s[j]
    if x == "(":
        stack.append(1)
    else:
        if not stack:
            temp += a
        else:
            stack.pop()

print(temp + a*len(stack))
