// O(Tmaxval)
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

    int n, q;
    cin >> n >> q;

    vvi countries(n);
    int m = 0;
    rep(i, n)
    {
        int k;
        cin >> k;
        countries[i].resize(k);
        rep(j, k) cin >> countries[i][j];
        rep(j, k) m = max(m, (int)countries[i][j]);
    }

    map<int, int> cache;
    auto solve = [&](int x)
    {
        if (cache.count(x)) return cache[x];
        p2 best = p2(0, n);

        rep(i, n)
        {
            int amount = 0;
            repe(v, countries[i]) amount += v >= x;
            best = max(best, p2(amount, i + 1));
        }

        return cache[x]=best.second;
    };

    while (q--)
    {
        int x;
        cin >> x;
        x = min(x, m+1);
        cout << solve(x) << '\n';
    }

    return 0;
}
