# @EXPECTED_GRADES@ TLE AC AC TLE TLE TLE TLE TLE
# O(N^3) DP: for each rotation, O(N^2) DP over (position, balance)

def solve():
    n, a, b = map(int, input().split())
    s = input()

    INF = float('inf')
    best = INF

    for c in range(n):
        dp = [INF] * (n + 1)
        dp[0] = 0
        lo = hi = 0

        for idx in range(n):
            pos = (c + idx) % n
            keep_cost = b if pos < c else 0
            is_open = s[pos] == '('

            ndp = [INF] * (n + 1)
            nlo, nhi = n, -1

            for j in range(lo, hi + 1):
                if dp[j] >= INF:
                    continue
                # Remove this character
                v = dp[j] + a
                if v < ndp[j]:
                    ndp[j] = v
                    if j < nlo: nlo = j
                    if j > nhi: nhi = j
                # Keep this character
                nj = j + 1 if is_open else j - 1
                if 0 <= nj <= n:
                    v = dp[j] + keep_cost
                    if v < ndp[nj]:
                        ndp[nj] = v
                        if nj < nlo: nlo = nj
                        if nj > nhi: nhi = nj

            dp = ndp
            lo, hi = nlo, nhi

        best = min(best, dp[0])

    print(best)

solve()
