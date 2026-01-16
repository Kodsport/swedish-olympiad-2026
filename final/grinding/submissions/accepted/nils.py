#!/usr/bin/python3

n,goal = map(int,input().split())

ans = goal
attacks = 0
strength = 1
ma_xp = 0

for i in range(n):
    m = int(input())
    S = []
    XP = []
    xp_tot = 0
    s_tot = 0
    for j in range(m):
        s, xp = map(int,input().split())
        s_tot = max(s_tot, s-xp_tot)
        xp_tot += xp

        if s_tot > strength:
            more = (s_tot-strength + ma_xp-1) // ma_xp
            attacks += more
            strength += ma_xp*more
        
        ma_xp = max(ma_xp, xp_tot)
        to_win = max(0,(goal-strength + xp_tot-1) // xp_tot)
        ans = min(ans, attacks + to_win)
    
    attacks += 1
    strength += xp_tot

print(ans)



