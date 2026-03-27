#pragma GCC optimize("O3")
#include <bits/allocator.h>
#pragma GCC target("avx2")
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

    // For random data, (0.2n, 0.8n) seems optimal
    // lets also try (1/3n, 2/3n) just to be safe
    vi candidatesS(all(endpoints));
    vi candidatesT(all(endpoints));
    sort(all(candidatesS), [&](int a, int b) {
        return abs(a-n/3) < abs(b-n/3);
    });

    sort(all(candidatesT), [&](int a, int b) {
        return abs(a-2*n/3) < abs(b-2*n/3);
    });

    // p2 best = p2(-1,-1);
    ll ans = inf;
    const ll BOUND = 500;
    rep(i,min(BOUND,sz(candidatesS))) {
        rep(j, min(BOUND,sz(candidatesT))) {
            int u = candidatesS[i];
            int v = candidatesT[j];
            ll cost = 0;
            #pragma GCC ivdep
            for (auto [a,b] : people) {
                cost += min({
                    abs(b-a), // a -> b
                    1+abs(a-u)+abs(b-v), // a->i, i->j, j->b
                    1+abs(a-v)+abs(b-u), // a->j, j->i, j->b
                });
            }
            // if (cost<ans) {
            //     best={u,v};
            // }
            ans = min(ans, cost);
        }
    }
    cout << ans << '\n';
    // cout << best.first << " " << best.second << '\n';

    return 0;
}
