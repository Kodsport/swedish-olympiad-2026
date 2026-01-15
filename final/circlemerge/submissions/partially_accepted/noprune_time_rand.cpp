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
        p[b] = a;
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);
    auto start = chrono::high_resolution_clock::now();

    int n;
    cin >> n;
    vi nums(n);
    repe(v, nums) cin >> v;
    ll tot = accumulate(all(nums),0LL);

    vi dup = nums;
    dup.insert(end(dup), all(dup));

    int checks = 0;
    auto check_possible = [&](ll num_groups) -> bool {
        if (num_groups > n) return false;
        if (tot % num_groups != 0) return false;
        ll target = tot / num_groups;
        if (tot % target != 0) return false;
        checks++;

        UF uf(n);
        int j = 0;
        int moddedj = 0;
        ll cur = 0;
        for (int i = 0; i < n && j < i + n; ++i) {
            while (j < i + n && cur < target) {
                cur += dup[j++];
                moddedj = (moddedj==n-1) ? 0 : moddedj+1;
            }
            if (cur == target) {
                int nxt = moddedj;
                if (uf.find(i) == uf.find(nxt)) return true;
                uf.merge(i, nxt);
            }
            cur -= dup[i];
        }
        return false;
    };

    vi order(n);
    repp(i, 1, n+1) order[i-1]=i;
    mt19937 rng(42);
    shuffle(all(order), rng);
    ll ans = 0;
    repe(num_groups, order) {
        if (num_groups<=ans) continue;
        auto curr = chrono::high_resolution_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(curr-start).count() > 2000) break;
        if (check_possible(num_groups)) ans = num_groups;
    }

    cout << n-ans << '\n';
    cerr << "Checks: " << checks << '\n';

    return 0;
}
