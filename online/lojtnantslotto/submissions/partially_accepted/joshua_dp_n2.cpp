#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<ll>;
using vvi = vector<vi>;
using p2 = pair<ll, ll>;
const ll inf = 1e18;

#define rep(i,n) for (ll i = 0; i < (n); i++)
#define repp(i,a,n) for (ll i = (a); i < (n); i++)
#define repe(i, arr) for (auto& i : arr)
#define all(x) begin(x),end(x)
#define sz(x) ((ll)(x).size())


int main()
{
    cin.tie(0)->sync_with_stdio(0);

    const int maxval = 5;

    int n, t;
    cin >> n >> t;
    vector<p2> castles(n);
    repe(c, castles) cin >> c.first >> c.second;

    vi dp(n * maxval + 1, inf);
    dp[0] = 0;
    for (auto [profit, weight] : castles)
    {
        for (int i = sz(dp) - 1; i - profit >= 0; i--)
        {
            dp[i] = min(dp[i], dp[i - profit] + weight);
        }
    }

    int ans = 0;
    rep(i, sz(dp)) if (dp[i] <= t) ans = i;

    cout << ans << '\n';


    return 0;
}
