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

    vvi occs(maxval+1);
    for (auto [profit, weight] : castles) occs[profit].push_back(weight);
    repe(o, occs) sort(all(o));

    rep(i, maxval) repp(j, 1, sz(occs[i])) occs[i][j] += occs[i][j - 1];

    int ans = 0;
    
    rep(i, sz(occs[1]))
    {
        if (occs[1][i] > t) break;
        int spare = t-occs[1][i];
        auto it = upper_bound(all(occs[2]), spare);
        int items = i + 1;
        items += 2*(it - occs[2].begin());
        ans = max(ans, items);
    }
    cout << ans << '\n';

    return 0;
}
