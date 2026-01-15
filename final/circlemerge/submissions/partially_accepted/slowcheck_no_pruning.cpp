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
    sort(all(divs));


    vi dup = nums;
    dup.insert(end(dup), all(dup));

    auto check_possible = [&](ll div)
    {
        ll target = tot / div;
        if (div > n) return false;
        if (!binary_search(all(divs), target)) return false;
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

    vvi divisors(n+1);
    repp(i,1,n+1) repp(j,1,n+1) {
        if (i*j>n) break;
        divisors[i*j].push_back(i);
    }
    ll ans = 0;

    vi order;
    repp(i,1,n+1) order.push_back(i);
    mt19937 rng(42);
    shuffle(all(order), rng);

    vi dead(n+1);
    repe(d, order) {
        if (d<=ans) continue;
        if (dead[d]) continue;
        if (check_possible(d)) ans = d;
        // TODO: pruning
    }

    cout << n-ans << '\n';

    return 0;
}
