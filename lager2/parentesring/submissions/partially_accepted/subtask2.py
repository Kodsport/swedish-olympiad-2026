# @EXPECTED_GRADES@ TLE AC TLE TLE TLE TLE TLE TLE
# Group 2: N<=100. Try all rotations, O(N^2) DP per rotation. O(N^3) total.
n, a, b = map(int, input().split())
s = input() * 2

best = float('inf')
for start in range(n):
    cost = start * b
    dp = {0: 0}
    for i in range(start, start + n):
        ndp = {}
        for j, v in dp.items():
            if j not in ndp or v + a < ndp[j]:
                ndp[j] = v + a
            nj = j + (1 if s[i] == '(' else -1)
            if nj >= 0 and (nj not in ndp or v < ndp[nj]):
                ndp[nj] = v
        dp = ndp
    cost += dp.get(0, float('inf'))
    best = min(best, cost)
print(best)
