// O(N*(divisors less than N))
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

struct UF
{
    vector<int> p;
    UF(int n) : p(n)
    {
        reset();
    }
    void reset()
    {
        iota(all(p), 0);
    }
    int find(int x) { return p[x] == x ? x : p[x]=find(p[x]); };
    void merge(int a, int b)
    {
        a = find(a); b = find(b);
        if (a==b) return;
        if (rand() % 2) swap(a,b);
        p[b] = a;
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vi nums(n);
    repe(v, nums) cin >> v;
    ll tot = accumulate(all(nums),0LL);

    int checks = 0;
    auto check_possible = [&](ll num_groups) -> bool {
        if (num_groups > n) return false;
        if (tot % num_groups != 0) return false;
        ll target = tot / num_groups;
        if (tot % target != 0) return false;
        checks++;

        UF uf(n);
        int moddedj = 0;
        ll cur = 0;
        for (int i = 0; i < n; ++i) {
            while (cur < target) {
                cur += nums[moddedj];
                moddedj = (moddedj==n-1) ? 0 : moddedj+1;
            }
            if (cur == target) {
                int nxt = moddedj;
                if (uf.find(i) == uf.find(nxt)) return true;
                uf.merge(i, nxt);
            }
            cur -= nums[i];
        }
        return false;
    };

    ll ans = 0;
    repp(num_groups, 1, n+1) {
        if (check_possible(num_groups)) ans = num_groups;
    }

    cout << n-ans << '\n';
    cerr << "Checks: " << checks << '\n';

    return 0;
}
