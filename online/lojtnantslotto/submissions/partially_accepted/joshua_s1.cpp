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

    int n, t;
    cin >> n >> t;
    vector<p2> castles(n);
    repe(c, castles) cin >> c.first >> c.second;

    sort(all(castles));

    int ans = 0;
    rep(i,n)
    {
        if (castles[i].second > t) break;
        t -= castles[i].second;
        ans++;
    }
    cout << ans << '\n';


    return 0;
}
