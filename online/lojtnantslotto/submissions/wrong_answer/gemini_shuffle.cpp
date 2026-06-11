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

const int maxval = 5;

int main() {
    cin.tie(0)->sync_with_stdio(0);

    auto start = chrono::high_resolution_clock::now();

    int n, t;
    cin >> n >> t;
    vector<p2> castles(n);
    repe(c, castles) cin >> c.first >> c.second;

    vvi occs(maxval + 1);
    for (auto [profit, weight] : castles) occs[profit].push_back(weight);
    repe(o, occs) sort(all(o));

    vvi occs_pref = occs;
    rep(i, maxval + 1) repp(j, 1, sz(occs_pref[i])) occs_pref[i][j] += occs_pref[i][j - 1];

    mt19937 rng(42);

    ll ans = 0;
    int iter = 0;
    while (1) {
        if (iter % 1000 == 0) {
            auto end = chrono::high_resolution_clock::now();
            if (chrono::duration_cast<chrono::milliseconds>(end - start).count() > 950) break;
        }
        iter++;

        // Generate 4 random cut points in [0, T] to partition T into 5 parts
        vector<ll> cuts(4);
        rep(i, 4) cuts[i] = uniform_int_distribution<ll>(0, t)(rng);
        sort(all(cuts));

        vector<ll> alloc(maxval + 1);
        alloc[1] = cuts[0];
        alloc[2] = cuts[1] - cuts[0];
        alloc[3] = cuts[2] - cuts[1];
        alloc[4] = cuts[3] - cuts[2];
        alloc[5] = t - cuts[3];

        ll current_profit = 0;
        repp(i, 1, maxval + 1) {
            if (occs_pref[i].empty()) continue;
            // Binary search to find how many castles of value i we can afford
            auto it = upper_bound(all(occs_pref[i]), alloc[i]);
            ll cnt = distance(occs_pref[i].begin(), it);
            current_profit += cnt * i;
        }

        ans = max(ans, current_profit);
    }

    cout << ans << '\n';

    return 0;
}
