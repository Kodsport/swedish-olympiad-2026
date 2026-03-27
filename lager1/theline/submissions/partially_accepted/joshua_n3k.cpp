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

    auto bfs = [&](int a, int b, vvi& adj) {
        if (a==b) return 0LL;
        vi vis(n);
        queue<p2> q;
        q.emplace(0, a);
        vis[a]=1;
        while (sz(q)) {
            auto [d,u] = q.front();
            q.pop();
            if (u==b) return d;
            repe(e,adj[u]) {
                if (!vis[e]) {
                    vis[e]=1;
                    q.emplace(d+1,e);
                }
            }
        }

        assert(0);
    };

    ll ans = inf;
    rep(i,n) {
        repp(j, i, n) {
            vvi adj(n);
            rep(k, n-1) adj[k].push_back(k+1), adj[k+1].push_back(k);
            adj[i].push_back(j);
            adj[j].push_back(i);
            ll cost = 0;
            for (auto [a,b] : people) {
                cost += bfs(a,b,adj);
            }
            ans = min(ans, cost);
        }
    }
    cout << ans << '\n';

    return 0;
}
