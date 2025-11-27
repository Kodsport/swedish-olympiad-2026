#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<int> par(n), depth(n, 0), parent(n);
    for (int i = 0; i < n; ++i) { 
        cin >> par[i]; --par[i]; 
    }

    int root = -1;
    for (int i = 0; i < n; ++i) if (par[i] == i) root = i;
    assert(root != -1);

    vector<vector<int>> adj(n);
    for (int i = 0; i < n; ++i) if (i != root) {
        adj[i].push_back(par[i]);
        adj[par[i]].push_back(i);
    }

    // DFS to compute depth and parent
    parent[root] = root; depth[root] = 0;
    stack<int> st; st.push(root);
    while (!st.empty()) {
        int v = st.top(); st.pop();
        for (int to : adj[v]) if (to != parent[v]) {
            parent[to] = v;
            depth[to] = depth[v] + 1;
            st.push(to);
        }
    }

    // Binary lifting LCA
    int LOG = 1;
    while ((1 << LOG) <= n) ++LOG;
    vector<vector<int>> up(LOG, vector<int>(n));
    for (int v = 0; v < n; ++v) up[0][v] = parent[v];
    for (int k = 1; k < LOG; ++k)
        for (int v = 0; v < n; ++v)
            up[k][v] = up[k-1][ up[k-1][v] ];

    auto lca_two = [&](int a, int b) {
        if (a == -1) return b;
        if (b == -1) return a;
        if (a == b) return a;
        if (depth[a] < depth[b]) swap(a,b);
        int diff = depth[a] - depth[b];
        for (int k = 0; k < LOG; ++k) if (diff & (1<<k)) a = up[k][a];
        if (a == b) return a;
        for (int k = LOG-1; k >= 0; --k) {
            if (up[k][a] != up[k][b]) {
                a = up[k][a]; b = up[k][b];
            }
        }
        return up[0][a];
    };

    vector<int> who_lives(n,-1), which_house(m);
    for (int i = 0; i < m; ++i) {
        int h; cin >> h; --h;
        which_house[i] = h;
        who_lives[h] = i;
    }

    auto mod = [&](int x){ x %= m; if (x < 0) x += m; return x; };

    auto swap_houses = [&](int a, int b){
        swap(who_lives[a], who_lives[b]);
        if (who_lives[a] != -1) which_house[who_lives[a]] = a;
        if (who_lives[b] != -1) which_house[who_lives[b]] = b;
    };

    auto expand_right = [&](int p, int radius){
        int cur_max = INT_MIN;
        int cur_lca = -1;
        for (int k = 0; k < m; ++k){
            int idx = p + k; if (idx >= m) idx -= m;
            int h = which_house[idx];
            cur_max = max(cur_max, depth[h]);
            cur_lca = lca_two(cur_lca, h);
            if (cur_max - depth[cur_lca] > radius) return k;
        }
        return m;
    };

    auto expand_left = [&](int p, int radius){
        int cur_max = INT_MIN;
        int cur_lca = -1;
        for (int k = 0; k < m; ++k){
            int idx = p - k; if (idx < 0) idx += m;
            int h = which_house[idx];
            cur_max = max(cur_max, depth[h]);
            cur_lca = lca_two(cur_lca, h);
            if (cur_max - depth[cur_lca] > radius) return k;
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
}
