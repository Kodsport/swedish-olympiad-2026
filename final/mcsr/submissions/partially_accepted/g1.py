
n,s = map(int,input().split())
pos = []
tim = []

for _ in range(n):
    x,t = map(int,input().split())
    pos.append(x)
    tim.append(t)

print(min(s, pos[0]*8 + tim[0] + (pos[-1]-pos[0]) + tim[-1] + (s - pos[-1]*8)))
