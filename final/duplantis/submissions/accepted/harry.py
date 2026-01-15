n,v = map(int, input().split())
a = [*map(int, input().split())]

suffmax = [0]*n
suffmax[-1] = a[-1]

for i in range(n-2,-1,-1):
    suffmax[i] = max(a[i],suffmax[i+1])

ans = []
for i in range(n):
    if a[i] > v:
        v += 1
        ans.append(suffmax[i])

print(len(ans))
print(*ans)
