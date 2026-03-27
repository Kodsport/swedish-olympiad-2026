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


int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n,k;
    cin >> n >> k;
    vector<p2> people(k);
    set<int> endpoints;
    for (auto& [a,b] : people) {
        cin >> a >> b;
        a--; b--;
        if (a>b) swap(a,b);
        endpoints.insert(a);
        endpoints.insert(b);
    }

    vi candidates(all(endpoints));

    ll ans = inf;
    rep(i,sz(candidates)) {
        repp(j, i, sz(candidates)) {
            int u = candidates[i];
            int v = candidates[j];
            ll cost = 0;
            for (auto [a,b] : people) {
                cost += min({
                    abs(b-a), // a -> b
                    1+abs(a-u)+abs(b-v), // a->i, i->j, j->b
                    1+abs(a-v)+abs(b-u), // a->j, j->i, j->b
                });
            }
            ans = min(ans, cost);
        }
    }
    cout << ans << '\n';

    return 0;
}
