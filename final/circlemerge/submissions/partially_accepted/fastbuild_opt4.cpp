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
    int d = 2;
    while (d * d <= x)
    {
        if (x % d == 0) factors[d]++, x /= d;
        else d++;
    }
    if (x > 1) factors[x]++;

    return factors;
}

void divisors(ll prod, map<ll, ll>::iterator it, map<ll, ll>& factors, vi& res)
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
    vi pref(dup);
    repp(i, 1, sz(pref)) pref[i] += pref[i - 1];

    auto rsq = [&](ll l, ll r)
    {
        if (l) l = pref[l - 1];
        return pref[r] - l;
    };

    ll ans = n;
    repe(div, divs)
    {
        if (div > n) continue;
        if (n - div >= ans) continue;
        ll target = tot / div;
        vector<int> nxt(2 * n, 2*n);
        queue<p2> waiting;
        ll lazyadd = 0;
        rep(i, 2 * n)
        {
            lazyadd += dup[i];
            // lazyadd + x = dup[i]
            // x = dup[i] - lazyadd
            if (i < n) waiting.emplace(i, dup[i] - lazyadd);
            while (sz(waiting) && waiting.front().second + lazyadd >= target)
            {
                auto [j, t] = waiting.front();
                waiting.pop();
                if (t+lazyadd==target)
                {
                    nxt[j] = i+1;
                    if (i+1+n<2*n) nxt[j+n] = i+n+1;
                }
            }
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
                ans = min(ans, n - div);
                break;
            }
        }
    }
    cout << ans << '\n';

    return 0;
}
