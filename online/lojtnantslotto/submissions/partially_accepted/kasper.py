from math import inf

N, K = map(int, input().split())
castles = [[] for _ in range(5)]
for _ in range(N):
    a, b = map(int, input().split())
    castles[a-1].append(b)
for c in castles: c.sort()

dp = [inf for _ in range(5*N+1)]
next = [[0 for i in range(5)] for _ in range(5*N+1)]
dp[0] = 0
for i in range(1, 5*N+1):
    for j in range(1, 6):
        if j > i: continue
        if next[i-j][j-1] == len(castles[j-1]): continue
        if dp[i-j] + castles[j-1][next[i-j][j-1]] < dp[i]:
            dp[i] = dp[i-j] + castles[j-1][next[i-j][j-1]]
            next[i][0] = next[i-j][0]
            next[i][1] = next[i-j][1]
            next[i][2] = next[i-j][2]
            next[i][3] = next[i-j][3]
            next[i][4] = next[i-j][4]
            next[i][j-1] += 1
ans = 0
for i in range(len(dp)):
    if dp[i] <= K:
        ans = i
print(ans)

