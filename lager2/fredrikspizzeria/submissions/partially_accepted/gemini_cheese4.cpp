#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>

using namespace std;

struct Edge {
    int u, v;
};

struct DSU {
    vector<int> p, sz;
    vector<pair<int, int>> hist;
    
    DSU(int n) : p(n + 1), sz(n + 1, 1) {
        iota(p.begin(), p.end(), 0);
    }
    
    int find(int x) {
        while (x != p[x]) x = p[x];
        return x;
    }
    
    bool merge(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        if (sz[u] < sz[v]) swap(u, v);
        p[v] = u;
        sz[u] += sz[v];
        hist.push_back({v, u});
        return true;
    }
    
    void rollback(int snaps) {
        while (hist.size() > snaps) {
            int v = hist.back().first;
            int u = hist.back().second;
            hist.pop_back();
            sz[u] -= sz[v];
            p[v] = v;
        }
    }
};

const int MAX_QUERIES = 500005;
vector<Edge> seg[4 * MAX_QUERIES];

void add_to_tree(int u, int l, int r, int ql, int qr, Edge e) {
    if (l > qr || r < ql) return;
    if (ql <= l && r <= qr) {
        seg[u].push_back(e);
        return;
    }
    int mid = l + (r - l) / 2;
    add_to_tree(2 * u, l, mid, ql, qr, e);
    add_to_tree(2 * u + 1, mid + 1, r, ql, qr, e);
}

int N, M;
bool found_ans = false;

void dfs(int u, int l, int r, DSU& dsu) {
    if (found_ans) return;
    int snaps = dsu.hist.size();
    
    for (auto e : seg[u]) dsu.merge(e.u, e.v);
    
    if (l == r) {
        if (dsu.find(1) == dsu.find(2) && dsu.find(1) != dsu.find(3)) {
            cout << "Ja\n";
            exit(0);
        }
    } else {
        int mid = l + (r - l) / 2;
        dfs(2 * u, l, mid, dsu);
        dfs(2 * u + 1, mid + 1, r, dsu);
    }
    
    dsu.rollback(snaps);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    if (!(cin >> N >> M)) return 0;
    
    vector<Edge> edges(M + 1);
    vector<int> paired(M + 1);
    vector<vector<pair<int, int>>> adj(N + 1);
    
    for (int i = 1; i <= M; i++) {
        int u, v, l;
        cin >> u >> v >> l;
        edges[i] = {u, v};
        paired[i] = l;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    
    // Step 1: Find all edges on paths from the rat (node 3)
    vector<int> candidates;
    vector<bool> vis(N + 1, false);
    queue<int> q;
    
    q.push(3);
    vis[3] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int idx = edge.second;
            
            // Standardize link ID by taking the minimum of the pair
            int link_id = min(idx, paired[idx]);
            candidates.push_back(link_id);
            
            // Stop traversing if we hit the start or exit to isolate the path
            if (!vis[v] && v != 1 && v != 2) {
                vis[v] = true;
                q.push(v);
            }
        }
    }
    
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());
    
    // Step 2: Sample up to K queries
    vector<pair<int, int>> queries;
    
    // Test cutting 0 edges (baseline check)
    queries.push_back({0, 0});
    
    // Test cutting 1 link (2 edges)
    for (int c : candidates) {
        queries.push_back({c, 0});
    }
    
    // Test cutting 2 links (4 edges)
    int K = 200000; 
    mt19937 rng(1337);
    
    if (1LL * candidates.size() * candidates.size() <= K * 2LL) {
        for (size_t i = 0; i < candidates.size(); ++i) {
            for (size_t j = i + 1; j < candidates.size(); ++j) {
                queries.push_back({candidates[i], candidates[j]});
            }
        }
    } else {
        for (int i = 0; i < K; ++i) {
            int u = candidates[rng() % candidates.size()];
            int v = candidates[rng() % candidates.size()];
            if (u != v) queries.push_back({u, v});
        }
    }
    
    int Q = queries.size();
    
    // Step 3: Fast Offline Dynamic Connectivity Setup
    vector<vector<int>> removed_in(M + 1);
    for (int i = 0; i < Q; ++i) {
        if (queries[i].first != 0) removed_in[queries[i].first].push_back(i);
        if (queries[i].second != 0) removed_in[queries[i].second].push_back(i);
    }
    
    vector<bool> is_candidate(M + 1, false);
    for (int c : candidates) is_candidate[c] = true;
    
    for (int i = 1; i <= M; i++) {
        // Only process the canonical representation of the link to avoid double adding
        if (min(i, paired[i]) != i) continue; 
        
        if (!is_candidate[i]) {
            // Edge is never removed, add to the entire timeline
            add_to_tree(1, 0, Q - 1, 0, Q - 1, edges[i]);
            add_to_tree(1, 0, Q - 1, 0, Q - 1, edges[paired[i]]);
        } else {
            auto& rem = removed_in[i];
            sort(rem.begin(), rem.end());
            rem.erase(unique(rem.begin(), rem.end()), rem.end());
            
            int last = 0;
            for (int q_idx : rem) {
                if (last <= q_idx - 1) {
                    add_to_tree(1, 0, Q - 1, last, q_idx - 1, edges[i]);
                    add_to_tree(1, 0, Q - 1, last, q_idx - 1, edges[paired[i]]);
                }
                last = q_idx + 1;
            }
            if (last <= Q - 1) {
                add_to_tree(1, 0, Q - 1, last, Q - 1, edges[i]);
                add_to_tree(1, 0, Q - 1, last, Q - 1, edges[paired[i]]);
            }
        }
    }
    
    // Step 4: D&C Traversal
    DSU dsu(N);
    dfs(1, 0, Q - 1, dsu);
    
    cout << "Nej\n";
    return 0;
}