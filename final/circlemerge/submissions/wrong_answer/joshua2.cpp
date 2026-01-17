// O(divs(N + ???))
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<ll>;
using vvi = vector<vi>;
using p2 = pair<ll, ll>;
const ll inf = 1e18;

#define repe(i, arr) for (auto& i : arr)
#define rep(i, b) for(ll i = 0; i < (b); ++i)
#define repp(i, a, b) for(ll i = a; i < (b); ++i)
#define all(x) begin(x),end(x)
#define sz(x) ((ll)x.size())

map<ll, ll> factor(ll x)
{
    map<ll, ll> factors;
    ll d = 2;
    while (d * d <= x)
    {
        if (x % d == 0) factors[d]++, x /= d;
        else d++;
    }
    if (x > 1) factors[x]++;

    return factors;
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vi nums(n);
    repe(v, nums) cin >> v;

    ll tot = accumulate(all(nums), 0LL);
    map<ll, ll> factors = factor(tot);

    vi dup = nums;
    repe(v, nums) dup.push_back(v);

    auto check_possible = [&](ll div)
    {
        ll target = tot / div;
        vector<int> nxt(2 * n, 2*n);
        int j = 0;
        ll t = 0;
        rep(i, 2 * n)
        {
            while (j < sz(dup) && t < target)
            {
                t += dup[j++];
            }
            if (t == target) nxt[i] = j;
            t -= dup[i];
        }

        rep(i, n)
        {
            int j = i;
            while (j < i + n)
            {
                j = nxt[j];
            }
            if (j == i+n)
            {
                return true;
            }
        }
        return false;
    };

    ll k = 0;
    repp(d, 1, n+1) if (tot % d == 0) k = max(k, d);

    // illegal pruning
    for (auto [p, cnt] : factors) {
        while (cnt-- > 0 && k % p == 0) {
            if (check_possible(k)) break;
            k /= p;
        }
    }
    
    if (check_possible(tot)) {
        cout << 0 << '\n';
        return 0;
    }

    ll ans = n-k;
    cout << ans << '\n';

    return 0;
}
