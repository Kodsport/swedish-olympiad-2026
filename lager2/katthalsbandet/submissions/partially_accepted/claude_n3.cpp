// @EXPECTED_GRADES@ TLE AC AC TLE TLE TLE TLE TLE
// O(N^3) DP: for each rotation, O(N^2) DP over (position, balance)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    int n;
    ll a, b;
    scanf("%d %lld %lld", &n, &a, &b);
    char s[500001];
    scanf("%s", s);

    const ll INF = 1e18;
    ll best = INF;
    vector<ll> dp(n + 1), ndp(n + 1);

    for (int c = 0; c < n; c++) {
        fill(dp.begin(), dp.begin() + n + 1, INF);
        dp[0] = 0;
        int lo = 0, hi = 0;

        for (int idx = 0; idx < n; idx++) {
            int pos = (c + idx) % n;
            ll keep = (pos < c) ? b : 0;
            bool open = s[pos] == '(';

            int nlo = n, nhi = -1;
            // Only clear the range we might write to
            for (int j = max(0, lo - 1); j <= min(n, hi + 1); j++)
                ndp[j] = INF;

            for (int j = lo; j <= hi; j++) {
                if (dp[j] >= INF) continue;
                // Remove
                ll v = dp[j] + a;
                if (v < ndp[j]) {
                    ndp[j] = v;
                    if (j < nlo) nlo = j;
                    if (j > nhi) nhi = j;
                }
                // Keep
                int nj = open ? j + 1 : j - 1;
                if (nj >= 0 && nj <= n) {
                    v = dp[j] + keep;
                    if (v < ndp[nj]) {
                        ndp[nj] = v;
                        if (nj < nlo) nlo = nj;
                        if (nj > nhi) nhi = nj;
                    }
                }
            }

            swap(dp, ndp);
            lo = nlo;
            hi = nhi;
        }

        best = min(best, dp[0]);
    }

    printf("%lld\n", best);
}
