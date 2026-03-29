#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 200005;
const int MAXM = 200005;

// ---- Disjoint Set Union (DSU) ----
int parent_dsu[MAXM];
int find_dsu(int i) {
    return parent_dsu[i] == i ? i : parent_dsu[i] = find_dsu(parent_dsu[i]);
}
void unite_dsu(int i, int j) {
    parent_dsu[find_dsu(i)] = find_dsu(j);
}
int link_group[MAXM];

// ---- Original Graph Variables ----
int U[MAXM], V[MAXM];
vector<pair<int, int>> adj[MAXN];

// ---- DFS & Cycle Extraction ----
bool vis[MAXN];
int depth[MAXN], pa_node[MAXN], pa_edge[MAXN];
vector<int> cyc_edges[MAXM];
int edge_cycle_id[MAXM];
int num_cycles = 0;

void dfs_cactus(int u, int p, int p_edge) {
    vis[u] = true;
    depth[u] = depth[p] + 1;
    pa_node[u] = p;
    pa_edge[u] = p_edge;

    for (auto& edge : adj[u]) {
        int v = edge.first, id = edge.second;
        if (v == p) continue;

        if (vis[v]) {
            if (depth[v] < depth[u]) { 
                num_cycles++;
                cyc_edges[num_cycles].push_back(id);
                edge_cycle_id[id] = num_cycles;
                int curr = u;
                while (curr != v) {
                    int ce = pa_edge[curr];
                    cyc_edges[num_cycles].push_back(ce);
                    edge_cycle_id[ce] = num_cycles;
                    curr = pa_node[curr];
                }
            }
        } else {
            dfs_cactus(v, u, id);
        }
    }
}

// ---- Explicit Block-Cut Tree (BCT) ----
vector<pair<int, int>> bct[MAXN * 2]; 
int bct_pa_node[MAXN * 2], bct_pa_edge[MAXN * 2];

void dfs_bct(int u, int p, int p_edge) {
    bct_pa_node[u] = p;
    bct_pa_edge[u] = p_edge;
    for (auto& edge : bct[u]) {
        int v = edge.first;
        if (v != p) dfs_bct(v, u, edge.second);
    }
}

struct Block {
    int type; // 0 for bridge, 1 for cycle
    int id, entry, exit;
};

// Traverse parents to extract the path structure cleanly
vector<Block> extract_path(int target, int N) {
    vector<Block> blocks;
    int curr = target;
    while (curr != 1 && curr != 0) {
        int p = bct_pa_node[curr];
        if (p <= N) { // Bridge
            blocks.push_back({0, bct_pa_edge[curr], p, curr});
            curr = p;
        } else { // Cycle Node
            int gp = bct_pa_node[p];
            blocks.push_back({1, p - N, gp, curr});
            curr = gp;
        }
    }
    reverse(blocks.begin(), blocks.end());
    return blocks;
}

vector<pair<int, int>> local_adj[MAXN];
void get_sides(int c, int u, int v, vector<int>& side_a, vector<int>& side_b) {
    vector<int> touched;
    for (int id : cyc_edges[c]) {
        local_adj[U[id]].push_back({V[id], id});
        local_adj[V[id]].push_back({U[id], id});
        touched.push_back(U[id]); touched.push_back(V[id]);
    }

    auto trace = [&](int start_idx, vector<int>& side) {
        int curr = local_adj[u][start_idx].first;
        int prev = u;
        side.push_back(local_adj[u][start_idx].second);
        while (curr != v) {
            for (auto& edge : local_adj[curr]) {
                if (edge.first != prev) {
                    side.push_back(edge.second);
                    prev = curr; curr = edge.first; break;
                }
            }
        }
    };
    trace(0, side_a); trace(1, side_b);
    for (int t : touched) local_adj[t].clear();
}

// ---- Constraint Satisfaction (SAT) Engine ----
bool is_bot[MAXM];
struct Constraint { int c_idx, side; };
vector<Constraint> g_forces[MAXM];
int assigned[MAXN]; // Tracks active 1->2 cycle choices

bool solve() {
    int N, M;
    if (!(cin >> N >> M)) return false;

    for (int i = 1; i <= M; ++i) parent_dsu[i] = i;
    for (int i = 1; i <= M; ++i) {
        int u, v, o; cin >> u >> v >> o;
        U[i] = u; V[i] = v;
        adj[u].push_back({v, i}); adj[v].push_back({u, i});
        unite_dsu(i, o);
    }
    for (int i = 1; i <= M; ++i) link_group[i] = find_dsu(i);

    // 1. Graph Analysis
    dfs_cactus(1, 0, 0);

    // 2. BCT Construction
    for (int i = 1; i <= M; ++i) {
        if (edge_cycle_id[i] == 0) { // Bridge
            bct[U[i]].push_back({V[i], i}); bct[V[i]].push_back({U[i], i});
        }
    }
    for (int c = 1; c <= num_cycles; ++c) { // Cycles
        vector<int> verts;
        for (int id : cyc_edges[c]) { verts.push_back(U[id]); verts.push_back(V[id]); }
        sort(verts.begin(), verts.end()); verts.erase(unique(verts.begin(), verts.end()), verts.end());
        int c_node = N + c;
        for (int v : verts) {
            bct[v].push_back({c_node, 0}); bct[c_node].push_back({v, 0});
        }
    }

    // 3. Tree Routing
    dfs_bct(1, 0, 0);
    vector<Block> path2 = extract_path(2, N);
    vector<Block> path3 = extract_path(3, N);

    if (path2.empty()) { cout << "Nej\n"; return true; }
    if (path3.empty()) { cout << "Ja\n"; return true; }

    // 4. SAT Constraint Generation (Escape Route)
    int var_idx = 0;
    for (auto& b : path2) {
        if (b.type == 0) {
            is_bot[link_group[b.id]] = true; // Mandatory bridge
        } else {
            vector<int> sA, sB;
            get_sides(b.id, b.entry, b.exit, sA, sB);
            for (int e : sA) g_forces[link_group[e]].push_back({var_idx, 1});
            for (int e : sB) g_forces[link_group[e]].push_back({var_idx, 0});
            var_idx++;
        }
    }

    // Resolve internal contradictions in requirements
    for (int i = 1; i <= M; ++i) {
        if (is_bot[i] || g_forces[i].empty()) continue;
        auto& f = g_forces[i];
        sort(f.begin(), f.end(), [](const Constraint& a, const Constraint& b) {
            return a.c_idx < b.c_idx;
        });
        bool local_conflict = false;
        for(size_t j = 1; j < f.size(); ++j) {
            if(f[j].c_idx == f[j-1].c_idx && f[j].side != f[j-1].side) local_conflict = true;
        }
        if (local_conflict) is_bot[i] = true;
    }

    // 5. SAT Checking (Rat Path Blockades)
    for (int i = 0; i < var_idx; ++i) assigned[i] = -1;

    bool can_block = false;
    for (auto& b : path3) {
        if (b.type == 0) {
            if (!is_bot[link_group[b.id]]) { can_block = true; break; }
        } else {
            vector<int> sA, sB;
            get_sides(b.id, b.entry, b.exit, sA, sB);
            
            // Optimization: Split constrained vs. unconstrained groups to avoid O(M^2)
            vector<int> gA_free, gA_cond, gB_free, gB_cond;
            auto process = [&](vector<int>& side, vector<int>& free, vector<int>& cond) {
                for(int e : side) {
                    int g = link_group[e];
                    if(!is_bot[g]) {
                        if(g_forces[g].empty()) free.push_back(g);
                        else cond.push_back(g);
                    }
                }
            };
            process(sA, gA_free, gA_cond); process(sB, gB_free, gB_cond);

            if (!gA_free.empty() && (!gB_free.empty() || !gB_cond.empty())) { can_block = true; break; }
            if (!gB_free.empty() && (!gA_free.empty() || !gA_cond.empty())) { can_block = true; break; }

            // Only strictly test combinations with active constraints
            for (int g1 : gA_cond) {
                for (int g2 : gB_cond) {
                    vector<int> touched; bool ok = true;
                    auto apply = [&](int g) {
                        for(auto& c : g_forces[g]) {
                            int req = 1 - c.side; 
                            if(assigned[c.c_idx] == -1) {
                                assigned[c.c_idx] = req; touched.push_back(c.c_idx);
                            } else if(assigned[c.c_idx] != req) return false;
                        }
                        return true;
                    };
                    if (apply(g1) && apply(g2)) {
                        can_block = true; 
                        for(int t : touched) assigned[t] = -1;
                        break;
                    }
                    for(int t : touched) assigned[t] = -1; // Rollback
                }
                if(can_block) break;
            }
        }
        if(can_block) break;
    }

    cout << (can_block ? "Ja\n" : "Nej\n");
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}