#include "validator.h"
using namespace std;

#include <numeric>
struct UF {
    vector<int> par;
    UF(int n) : par(n) {
        iota(begin(par), end(par), 0);
    }
    int find(int x) {return par[x]==x?x:par[x]=find(par[x]);}
    void merge(int a, int b) {
        a=find(a); b=find(b);
        par[b]=a;
    }
};

// Checks if every edge belongs to at most one cycle
bool is_edge_cactus(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> vis(n, 0), cov(n, 0);
    bool ok = true;
    
    auto dfs = [&](auto& self, int u, int p) -> void {
        vis[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (vis[v] == 1) { // Back-edge found
                cov[u]++; 
                cov[v]--;
            } else if (!vis[v]) {
                self(self, v, u);
                cov[u] += cov[v];
            }
        }
        vis[u] = 2;
        if (cov[u] > 1) ok = false;
    };
    
    if (n > 0) dfs(dfs, 0, -1);
    return ok;
}

// Checks if every vertex belongs to at most one cycle
bool is_vertex_cactus(const vector<vector<int>>& adj) {
    int n = adj.size();
    vector<int> vis(n, 0), cov(n, 0), back_deg(n, 0), p(n, -1);
    bool edge_cactus = true;
    
    auto dfs = [&](auto& self, int u, int par) -> void {
        vis[u] = 1;
        p[u] = par;
        for (int v : adj[u]) {
            if (v == par) continue;
            if (vis[v] == 1) { // Back-edge found
                cov[u]++; 
                cov[v]--;
                back_deg[u]++; 
                back_deg[v]++;
            } else if (!vis[v]) {
                self(self, v, u);
                cov[u] += cov[v];
            }
        }
        vis[u] = 2;
        if (cov[u] > 1) edge_cactus = false;
    };
    
    if (n > 0) dfs(dfs, 0, -1);
    if (!edge_cactus) return false;
    
    // Check cycle degrees: > 2 cycle-edges means > 1 cycle
    vector<int> cyc_deg = back_deg;
    for (int i = 0; i < n; i++) {
        if (p[i] != -1 && cov[i] == 1) {
            cyc_deg[i]++;
            cyc_deg[p[i]]++;
        }
        if (cyc_deg[i] > 2) return false;
    }
    
    return true;
}

const int MAX_N = 2e5;
const int MAX_M = 2e5;


void run() {
    int n = Int(3, Arg("maxn", MAX_N));
    Space();
    int m = Int(3, Arg("maxm", MAX_M));
    Endl();

    assert(m%2==0);
    
    UF uf(n);
    int ncomps = n;
    vector<vector<int>> adj(n);
    vector<int> edge_partner(m, -1);
    set<pair<int,int>> seen_edges;
    for (int i = 0; i < m; i++) {
        int u = Int(1, n);
        Space();
        int v = Int(1, n);
        Space();
        int ind = Int(1, m);
        Endl();
        edge_partner[i] = ind-1;

        assert(u!=v);

        u--;
        v--;
        pair<int,int> e = {min(u,v),max(u,v)};
        assert(!seen_edges.count(e));
        seen_edges.insert(e);

        adj[u].push_back(v);
        adj[v].push_back(u);

        if (uf.find(u) != uf.find(v)) ncomps--;
        uf.merge(u,v);
    }
    assert(ncomps==1);

    // Ensure that linked edges are symmetric
    for (int i = 0; i < m; i++) {
        assert(edge_partner[i] != i);
        assert(edge_partner[edge_partner[i]] == i);
    }

    if ((bool)Arg("tree", false)) {
        UF uf(n);
        for (auto [u,v] : seen_edges) {
            assert(uf.find(u) != uf.find(v));
            uf.merge(u,v);
        }
    }

    assert(is_edge_cactus(adj));
    if ((bool)Arg("vertexcactus", false)) {
        assert(is_vertex_cactus(adj));
    }
}
