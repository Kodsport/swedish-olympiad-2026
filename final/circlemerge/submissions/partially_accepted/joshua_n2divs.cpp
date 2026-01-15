// O(N^2 divs)
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

map<ll,ll> factor(ll x)
{
    map<ll, ll> factors;
    int d = 2;
    while (d*d<=x)
    {
        if (x % d == 0) factors[d]++, x /= d;
        else d++;
    }
    if (x > 1) factors[x]++;

    return factors;
}

void divisors(ll prod, map<ll,ll>::iterator it, map<ll, ll>& factors, vi& res)
{
    
    if (it == end(factors))
    {
        res.push_back(prod);
        return;
    }
    divisors(prod, next(it), factors, res);

    rep(i, it->second)
    {
        prod *= it->first;
        divisors(prod, next(it), factors, res);
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vi nums(n);
    repe(v, nums) cin >> v;

    ll tot = accumulate(all(nums), 0LL);
    map<ll, ll> factors = factor(tot);
    vi divs;
    divisors(1, begin(factors), factors, divs);

    vi dup = nums;
    repe(v, nums) dup.push_back(v);

    int ans = n;
    repe(div, divs)
    {
        ll target = tot / div;
        rep(i, n)
        {
            bool works = 1;
            int j = i;
            ll t = 0;
            int splits = 0;
            while (j<i+n)
            {
                if (t + dup[j] > target)
                {
                    works = 0;
                    break;
                }
                else if (t + dup[j] == target)
                {
                    t = 0;
                    splits++;
                }
                else t += dup[j];
                j++;
            }
            if (t != 0) continue;
            if (works)
            {
                ans = min(ans, n - splits);
            }
        }
    }
    cout << ans << '\n';

    return 0;
}
