
n,s = map(int,input().split())
pos = []
tim = []

for _ in range(n):
    x,t = map(int,input().split())
    pos.append(x)
    tim.append(t)

ans = s

out = n-1

for enter in range(n-2,-1,-1):
    ans = min(ans, 
        pos[enter]*8 + tim[enter] + (pos[out]-pos[enter]) + tim[out] + (s-pos[out]*8))

    if tim[enter] + (s-pos[enter]*8) < (pos[out]-pos[enter]) + tim[out] + (s-pos[out]*8):
        out = enter

print(ans)
