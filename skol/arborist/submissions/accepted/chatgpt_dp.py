#!/usr/bin/python3
# lol chatgpt writes 3^n DP instead of greedy (or 2^n*n^2)
import sys

def main():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    N = int(next(it)); K = int(next(it))
    xs = [int(next(it)) for _ in range(N)]
    ws = [int(next(it)) for _ in range(N)]

    INF = 10**9
    ALL = (1<<N) - 1

    # För varje subset kolla om den är genomförbar (vikt<=K) och beräkna kostnad
    subset_cost = [INF] * (1<<N)
    for mask in range(1, 1<<N):
        total_w = 0
        max_x = 0
        cnt = 0
        ok = True
        for i in range(N):
            if (mask >> i) & 1:
                total_w += ws[i]
                if ws[i] > K:
                    ok = False
                    break
                if xs[i] > max_x:
                    max_x = xs[i]
                cnt += 1
        if ok and total_w <= K:
            subset_cost[mask] = 2*max_x + 2*cnt

    # DP över mask - dp[mask] = min kostnad för att plantera de träd som mask har 1
    dp = [INF] * (1<<N)
    dp[0] = 0
    for mask in range(1<<N):
        if dp[mask] == INF: continue
        remaining = (~mask) & ALL
        # iterate submasks of remaining
        sub = remaining
        while sub:
            c = subset_cost[sub]
            if c < INF:
                new = mask | sub
                if dp[new] > dp[mask] + c:
                    dp[new] = dp[mask] + c
            sub = (sub - 1) & remaining

    print(dp[ALL])

if __name__ == "__main__":
    main()
