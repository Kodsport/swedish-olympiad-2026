#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>

using namespace std;

struct Edge {
    int u, v;
};

const int INF = 1e9;

// The blog's Weighted DSU adapted for our needs
struct DSU {
    vector<int> parent, weight, index;
 
    DSU(int n) : parent(n + 1), weight(n + 1, INF), index(n + 1) {
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
            index[i] = rand();
        }
    }
 
    int getRoot(int v, int w = INF - 1) {
        while (weight[v] <= w) {
            while (weight[parent[v]] <= weight[v])
                parent[v] = parent[parent[v]];
            v = parent[v];
        }
        return v;
    }
 
    void addEdgeHelper(int u, int v, int w) {
        while (u != v) {
            u = getRoot(u, w);
            v = getRoot(v, w);
            if (index[u] < index[v])
                swap(u, v);
            int temp_weight = weight[v], temp_parent = parent[v];
            parent[v] = u;
            weight[v] = w;
            u = temp_parent;
            w = temp_weight;
        }
    }

    int mainEdge(int u, int v) {
        if (getRoot(u) != getRoot(v))
            return -1;
        while (parent[u] != v && parent[v] != u) {
            if (weight[u] < weight[v])
                u = parent[u];
            else
                v = parent[v];
        }
        if (parent[u] == v)
            return u;
        else
            return v;
    }
 
    void addEdge(int u, int v, int w) {
        if (u == v)
            return;
        int p = mainEdge(u, v);
        if (p == -1)
            addEdgeHelper(u, v, w);
        else if (weight[p] > w) {
            parent[p] = p;
            weight[p] = INF;
            addEdgeHelper(u, v, w);
        }
    }
};

const int K = 700000; 
const int MAX_QUERIES = K+100005;

struct Event {
    int u, v, w;
};

vector<Event> adds[MAX_QUERIES];

// Intercept the segments directly at the root. We completely bypass the O(log Q) D&C tree routing.
void add_to_tree(int u, int l, int r, int ql, int qr, Edge e) {
    if (u == 1 && ql <= qr) {
        // We use -qr as the weight so the Weighted DSU (which maintains an MST) 
        // will naturally maintain a Maximum Spanning Tree of deletion times.
        adds[ql].push_back({e.u, e.v, -qr});
    }
}

int N, M;
bool found_ans = false;

// Replaces the Divide & Conquer traversal with a linear sweep over time.
void dfs(int u, int l, int r, DSU& dsu) {
    if (u != 1) return; // Only execute on the root call
    
    int Q = r + 1; 
    
    // Process queries linearly
    for (int t = 0; t < Q; t++) {
        // 1. Online Edge Insertion (Add edges becoming active at time t)
        for (auto& ev : adds[t]) {
            dsu.addEdge(ev.u, ev.v, ev.w);
        }
        
        // 2. Query Reachability
        // -t implies we only traverse edges whose deletion time (-qr) is <= -t (which means qr >= t).
        int r1 = dsu.getRoot(1, -t);
        int r2 = dsu.getRoot(2, -t);
        int r3 = dsu.getRoot(3, -t);
        
        // 3. Fast Checks
        if (r1 == r2 && r1 != r3) {
            cout << "Ja\n";
            exit(0);
        }
    }
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