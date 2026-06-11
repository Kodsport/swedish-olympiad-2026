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

ll ans = 0;
ll n, cap;

void rec(int i, ll profit, ll weight, vector<p2>& items)
{
    ans = max(ans, profit);
    if (i >= sz(items)) return;

    ll w = weight;
    ll p = profit;
    int j = i;
    while (j < sz(items)) {
        if (w + items[j].second > cap) break;
        p += items[j].first;
        w += items[j].second;
        j++;
    }
    
    if (j < sz(items)) {
        ll remaining = cap - w;
        p += items[j].first * remaining / (double)items[j].second;
    }

    if (p <= ans) return;

    if (weight + items[i].second <= cap) rec(i + 1, profit + items[i].first, weight + items[i].second, items);
    rec(i + 1, profit, weight, items);
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);

    cin >> n >> cap;
    vector<p2> items(n);
    for (auto& [p, w] : items) cin >> p >> w;
    sort(all(items), [&](const p2& a, const p2& b)
    {
        return a.first * b.second > b.first * a.second;
    });

    rec(0, 0, 0, items);
    cout << ans << '\n';

    return 0;
}
