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
    for (auto& [a,b] : people) {
        cin >> a >> b;
        a--; b--;
        if (a>b) swap(a,b);
    }

#if WHICH
    p2 best = p2(-1,-1);
#endif
    ll ans = inf;
    rep(i,n) {
        repp(j, i, n) {
            ll cost = 0;
            for (auto [a,b] : people) {
                cost += min({
                    abs(b-a), // a -> b
                    1+abs(a-i)+abs(b-j), // a->i, i->j, j->b
                    1+abs(a-j)+abs(b-i), // a->j, j->i, j->b
                });
            }
#if WHICH
            if (cost<ans) {
                best={i,j};
            }
#endif
            ans = min(ans, cost);
        }
    }
    cout << ans << '\n';
#if WHICH
    cout << best.first << " " << best.second << '\n';
#endif

    return 0;
}
