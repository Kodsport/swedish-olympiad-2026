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

struct Event {
    ll x;
    ll delta;
};

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

    vvi savings(n, vi(n));

    rep(i,n) {
        vector<vector<Event>> events(n);
        for (auto [a,b] : people) {
            ll y_f1 = abs(b-a)-1-abs(a-i);
            ll apex_f1 = b;
            // abs(x-apex_f1)+y_f1
            p2 f1_inter = {max(0LL, apex_f1-y_f1), min<ll>(n-1, apex_f1+y_f1)};
            
            // fn 1: (1+abs(a-i)) + abs(b-j)

            ll y_f2 = abs(b-a)-1-abs(b-i);
            ll apex_f2 = a;
            // abs(x-apex_f1)+y_f1
            p2 f2_inter = {max(0LL, apex_f2-y_f2), min<ll>(n-1, apex_f2+y_f2)};
            
            bitset<3000> active;
            repp(j,f1_inter.first,f1_inter.second+1) active[j] = 1;
            repp(j,f2_inter.first,f2_inter.second+1) active[j] = 1;
            
            for (int j = active._Find_first(); j < sz(active); j = active._Find_next(j)) {
                savings[i][j] += max({
                    y_f1-abs(j-apex_f1),
                    y_f2-abs(j-apex_f2)
                });
            }
        }
    }

    ll ans = -inf;
    rep(i,n) {
        repp(j, i, n) {
            ans = max(ans, savings[i][j]);
        }
    }
    ans = -ans;
    for (auto [a,b] : people) ans += abs(a-b);
    cout << ans << '\n';

    return 0;
}
