// O(N(chains log(divisors)))
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

// Chatgpt code: find min chain cover
namespace DivisorChains {

    struct HopcroftKarp {
        int L, R;
        vector<vector<int>> adj;
        vector<int> dist, pairU, pairV;
        const int INF = 1e9;

        HopcroftKarp(int L_ = 0, int R_ = 0) : L(L_), R(R_), adj(L_), dist(L_), pairU(L_, -1), pairV(R_, -1) {}

        void reset(int L_, int R_) {
            L = L_; R = R_;
            adj.assign(L, {});
            dist.assign(L, 0);
            pairU.assign(L, -1);
            pairV.assign(R, -1);
        }

        void addEdge(int u, int v) {
            adj[u].push_back(v);
        }

        bool bfs() {
            queue<int> q;
            for (int u = 0; u < L; ++u) {
                if (pairU[u] == -1) { dist[u] = 0; q.push(u); }
                else dist[u] = INF;
            }
            bool reachableFree = false;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    int u2 = pairV[v];
                    if (u2 != -1 && dist[u2] == INF) {
                        dist[u2] = dist[u] + 1;
                        q.push(u2);
                    }
                    if (u2 == -1) reachableFree = true;
                }
            }
            return reachableFree;
        }

        bool dfs(int u) {
            for (int v : adj[u]) {
                int u2 = pairV[v];
                if (u2 == -1 || (dist[u2] == dist[u] + 1 && dfs(u2))) {
                    pairU[u] = v;
                    pairV[v] = u;
                    return true;
                }
            }
            dist[u] = INF;
            return false;
        }

        int maxMatching() {
            int matching = 0;
            while (bfs()) {
                for (int u = 0; u < L; ++u) {
                    if (pairU[u] == -1 && dfs(u)) matching++;
                }
            }
            return matching;
        }
    };

    // Computes S = sorted vector of divisors of d that are <= n.
    static vector<int> build_divisor_set(int n, long long d) {
        vector<int> S;
        S.reserve(min(n, 100000));
        for (int x = 1; x <= n; ++x) {
            if (d % x == 0) S.push_back(x);
        }
        sort(S.begin(), S.end());
        return S;
    }

    // Main API: given n and d, return a vector<int> containing the top (maximum) element of each chain
    // in a minimum chain decomposition of S = {x <= n | x | d}.
    // The returned vector is sorted in descending order (largest top first).
    vector<vector<int>> compute_chain_tops(int n, long long d) {
        // Build S
        vector<int> S = build_divisor_set(n, d);
        int m = (int)S.size();
        if (m == 0) return {};

        // Map value -> index (0..m-1)
        unordered_map<int, int> id;
        id.reserve(m * 2);
        for (int i = 0; i < m; ++i) id[S[i]] = i;

        // Build HK and edges by multiples
        HopcroftKarp hk(m, m);
        for (int i = 0; i < m; ++i) {
            long long x = S[i];
            for (long long y = 2LL * x; y <= n; y += x) {
                auto it = id.find((int)y);
                if (it != id.end()) hk.addEdge(i, it->second);
            }
        }

        // Maximum matching
        hk.maxMatching();

        // Reconstruct chains. We treat left and right indices as the same ground set
        // (left index i corresponds to element S[i], and right index i corresponds to the same element)
        vector<char> visited(m, 0);
        vector<vector<int>> chains;
        // nodes with no matched incoming (pairV[i] == -1) are starts
        for (int i = 0; i < m; ++i) {
            if (hk.pairV[i] == -1) {
                vector<int> chain;
                int cur = i;
                while (cur != -1 && !visited[cur]) {
                    visited[cur] = 1;
                    chain.push_back(S[cur]);
                    int nxt = hk.pairU[cur];
                    if (nxt == -1) break;
                    cur = nxt;
                }
                chains.push_back(std::move(chain));
            }
        }
        // Any unvisited nodes -> singleton chains (safety)
        for (int i = 0; i < m; ++i) if (!visited[i]) chains.push_back(vector<int>{i});

        return chains;
    }

    // Convenience: return minimum chain count (width)
    int min_chain_count(int n, long long d) {
        return (int)compute_chain_tops(n, d).size();
    }

} // namespace DivisorChains

// Example usage (uncomment to run standalone)
/*
int main() {
    int n; long long d;
    if (!(cin >> n >> d)) return 0;
    auto tops = DivisorChains::compute_chain_tops(n, d);
    cout << "min chain count = " << tops.size() << "\n";
    cout << "chain tops (desc):";
    for (int v : tops) cout << ' ' << v;
    cout << '\n';
    return 0;
}
*/


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
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); };
    void merge(int a, int b)
    {
        a = find(a); b = find(b);
        if (a == b) return;
        p[b] = a;
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);

#if _LOCAL
    ifstream in("e:/in.txt");
    cin.rdbuf(in.rdbuf());
#endif

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

    ll ans = 0;
    vi memo(n + 1, -1);
    int checks = 0;
    auto check_possible = [&](ll num_groups) -> bool {
        if (num_groups <= ans) return true;
        if (num_groups > n) return false;
        if (tot % num_groups != 0) return false;
        ll target = tot / num_groups;
        if (!binary_search(all(divs), target)) return false;
        if (memo[num_groups] != -1) return memo[num_groups];
        checks++;

        UF uf(n);
        int j = 0;
        int moddedj = 0;
        ll cur = 0;
        for (int i = 0; i < n; ++i) {
            while (j < i + n && cur < target) {
                cur += dup[j++];
                moddedj = (moddedj == n - 1) ? 0 : moddedj + 1;
            }
            if (cur == target) {
                int nxt = moddedj;
                if (uf.find(i) == uf.find(nxt))
                {
                    ans = max(ans, num_groups);
                    return memo[num_groups] = true;
                }
                uf.merge(i, nxt);
            }
            cur -= dup[i];
        }
        return memo[num_groups]=false;
    };

    auto chains = DivisorChains::compute_chain_tops(n + 1, tot);
    sort(all(chains));
    reverse(all(chains));
    repe(chain, chains) {
        if (*begin(chain) == 0) chain.erase(begin(chain));
        if (chain.empty()) continue;
        auto it = upper_bound(all(chain), ans);
        if (it == chain.end()) continue;
        if (!check_possible(*it)) continue;
        int lo = it - begin(chain);
        int hi = sz(chain);
        while (lo+1<hi)
        {
            int mid = (lo + hi) / 2;
            if (check_possible(chain[mid])) lo = mid;
            else hi = mid;
        }
    }

    cout << n - ans << '\n';
    cerr << "checks: " << checks << '\n';

    return 0;
}
