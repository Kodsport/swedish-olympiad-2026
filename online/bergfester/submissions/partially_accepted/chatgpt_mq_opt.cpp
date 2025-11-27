#include <bits/stdc++.h>
using namespace std;

struct EulerLCA {
    int n;
    vector<int> depth;        // depth per node
    vector<int> first;        // first occurrence in euler tour
    vector<int> euler;        // euler tour (nodes)
    vector<vector<int>> st;   // sparse table storing node-id with min depth
    vector<int> lg;           // precomputed logs

    EulerLCA() = default;

    // build from adjacency list and chosen root
    EulerLCA(const vector<vector<int>>& adj, int root) {
        n = (int)adj.size();
        depth.assign(n, 0);
        first.assign(n, -1);
        euler.clear();

        // do DFS (recursive). If recursion depth is a concern, convert to iterative.
        function<void(int,int)> dfs = [&](int v, int p) {
            first[v] = (int)euler.size();
            euler.push_back(v);
            for (int to : adj[v]) if (to != p) {
                depth[to] = depth[v] + 1;
                dfs(to, v);
                euler.push_back(v);
            }
        };

        dfs(root, -1);

        // build log table
        int L = (int)euler.size();
        lg.assign(L + 1, 0);
        for (int i = 2; i <= L; ++i) lg[i] = lg[i/2] + 1;

        // build sparse table: st[0] = euler (node ids). For higher levels, keep node with smaller depth.
        int K = lg[L] + 1;
        st.assign(K, vector<int>(L));
        for (int i = 0; i < L; ++i) st[0][i] = euler[i];
        for (int k = 1; k < K; ++k) {
            int len = 1 << (k-1);
            for (int i = 0; i + 2*len <= L; ++i) {
                int a = st[k-1][i];
                int b = st[k-1][i + len];
                st[k][i] = (depth[a] < depth[b]) ? a : b;
            }
            // note: positions i such that i + 2*len > L are unused
        }
    }

    // returns LCA of a and b. supports a or b == -1 (returns the other)
    int lca(int a, int b) const {
        if (a == -1) return b;
        if (b == -1) return a;
        if (a == b) return a;
        int l = first[a], r = first[b];
        if (l > r) swap(l, r);
        // query [l, r] inclusive -> we want min depth node in that range
        int len = r - l + 1;
        int k = lg[len];
        int powk = 1 << k;
        int x = st[k][l];
        int y = st[k][r - powk + 1];
        return (depth[x] < depth[y]) ? x : y;
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    if (!(cin >> n >> m >> q)) return 0;

    vector<int> par(n);
    for (int i = 0; i < n; ++i) { cin >> par[i]; --par[i]; }

    int root = -1;
    for (int i = 0; i < n; ++i) if (par[i] == i) root = i;
    assert(root != -1);

    vector<vector<int>> adj(n);
    for (int i = 0; i < n; ++i) if (i != root) {
        adj[i].push_back(par[i]);
        adj[par[i]].push_back(i);
    }

    // We'll compute parent & depth with stack (so we also have a usable depth[] before building EulerLCA)
    vector<int> parent(n), depth(n, 0);
    parent[root] = root; depth[root] = 0;
    {
        stack<int> st; st.push(root);
        while (!st.empty()) {
            int v = st.top(); st.pop();
            for (int to : adj[v]) if (to != parent[v]) {
                parent[to] = v;
                depth[to] = depth[v] + 1;
                st.push(to);
            }
        }
    }

    // Build Euler + RMQ LCA structure (it computes its own depth vector; we could reuse our depth if preferred).
    EulerLCA elca(adj, root);

    vector<int> who_lives(n, -1), which_house(m);
    for (int i = 0; i < m; ++i) {
        int h; cin >> h; --h;
        which_house[i] = h;
        who_lives[h] = i;
    }

    auto swap_houses = [&](int a, int b){
        swap(who_lives[a], who_lives[b]);
        if (who_lives[a] != -1) which_house[who_lives[a]] = a;
        if (who_lives[b] != -1) which_house[who_lives[b]] = b;
    };

    // Now replace expand_right/left to use elca.lca and elca.depth
    auto expand_right = [&](int p, int radius){
        int cur_max = INT_MIN;
        int cur_lca = -1;
        for (int k = 0; k < m; ++k){
            int idx = p + k; if (idx >= m) idx -= m;
            int h = which_house[idx];
            // 'h' is a node index (house)
            cur_max = max(cur_max, elca.depth[h]);
            cur_lca = elca.lca(cur_lca, h);
            int cur_lca_depth = (cur_lca == -1) ? INT_MAX : elca.depth[cur_lca];
            if (cur_max - cur_lca_depth > radius) return k;
        }
        return m;
    };

    auto expand_left = [&](int p, int radius){
        int cur_max = INT_MIN;
        int cur_lca = -1;
        for (int k = 0; k < m; ++k){
            int idx = p - k; if (idx < 0) idx += m;
            int h = which_house[idx];
            cur_max = max(cur_max, elca.depth[h]);
            cur_lca = elca.lca(cur_lca, h);
            int cur_lca_depth = (cur_lca == -1) ? INT_MAX : elca.depth[cur_lca];
            if (cur_max - cur_lca_depth > radius) return k;
        }
        return m;
    };

    while (q--){
        int t; cin >> t;
        if (t == 1){
            int a,b; cin >> a >> b; --a; --b;
            swap_houses(a,b);
        } else {
            int p,radius; cin >> p >> radius; --p;
            int best = max(expand_right(p,radius), expand_left(p,radius));
            cout << best << '\n';
        }
    }

    return 0;
}
