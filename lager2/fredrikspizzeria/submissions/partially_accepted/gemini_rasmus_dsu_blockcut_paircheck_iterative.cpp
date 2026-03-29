#include <iostream>
#include <array>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>

using namespace std;

struct Edge {
    int u, v, id;
};

const int MAXM = 200005;
int link_group[MAXM];

// DSU to find equivalent corridor groups
struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        for (int i = 0; i <= n; ++i) parent[i] = i;
    }
    int find(int i) {
        int root = i;
        while (parent[root] != root) root = parent[root];
        while (parent[i] != i) {
            int p = parent[i];
            parent[i] = root;
            i = p;
        }
        return root;
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) parent[root_i] = root_j;
    }
};

vector<Edge> adj[MAXM];
bool vis[MAXM];
int depth[MAXM], pa[MAXM], pa_edge[MAXM];
vector<vector<int>> cycles;
int edge_to_cycle[MAXM];
vector<int> bridges;
int edge_to_bridge[MAXM];

struct DfsFrame {
    int u;
    int parent;
    int parent_edge;
    int next_idx;
};

// Iterative DFS to identify cycles and bridges in the cactus graph.
void dfs_iterative(int start) {
    vector<DfsFrame> st;
    st.push_back({start, 0, 0, 0});
    vis[start] = true;
    depth[start] = 1;
    pa[start] = 0;
    pa_edge[start] = 0;

    while (!st.empty()) {
        DfsFrame& frame = st.back();
        int u = frame.u;

        if (frame.next_idx == (int)adj[u].size()) {
            st.pop_back();
            continue;
        }

        const Edge& edge = adj[u][frame.next_idx++];
        int v = edge.v;
        int id = edge.id;
        if (v == frame.parent) continue;

        if (vis[v]) {
            if (depth[v] < depth[u]) {
                vector<int> path;
                path.push_back(id);
                int curr = u;
                while (curr != v) {
                    path.push_back(pa_edge[curr]);
                    curr = pa[curr];
                }
                cycles.push_back(path);
                int c_id = cycles.size();
                for (int e : path) edge_to_cycle[e] = c_id;
            }
            continue;
        }

        vis[v] = true;
        depth[v] = depth[u] + 1;
        pa[v] = u;
        pa_edge[v] = id;
        st.push_back({v, u, id, 0});
    }
}

// BCT variables
vector<int> bct_adj[MAXM * 3];
int bct_pa[MAXM * 3];

vector<int> get_bct_path(int start, int end) {
    queue<int> q;
    vector<bool> bct_vis(MAXM * 3, false);
    q.push(start);
    bct_vis[start] = true;
    bct_pa[start] = 0;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == end) break;
        for (int v : bct_adj[u]) {
            if (!bct_vis[v]) {
                bct_vis[v] = true;
                bct_pa[v] = u;
                q.push(v);
            }
        }
    }
    vector<int> path;
    if (!bct_vis[end]) return path;
    int curr = end;
    while (curr != 0) {
        path.push_back(curr);
        curr = bct_pa[curr];
    }
    vector<int> rev_path(path.rbegin(), path.rend());
    return rev_path;
}

int U[MAXM], V[MAXM];
bool is_bot[MAXM]; // True if group is impossible to keep closed
vector<pair<int, int>> group_req[MAXM]; // cycle_index -> required_value (0 or 1)

// Helper to extract the two paths of a cycle between entry and exit
void extract_cycle_paths(int c_id, int start, int end, vector<int>& pathA, vector<int>& pathB) {
    if (start == end) return;
    const vector<int>& cyc = cycles[c_id - 1];
    vector<int> verts;
    verts.reserve(cyc.size());
    for (int e : cyc) {
        verts.push_back(U[e]);
        verts.push_back(V[e]);
    }
    sort(verts.begin(), verts.end());
    verts.erase(unique(verts.begin(), verts.end()), verts.end());

    vector<array<pair<int, int>, 2>> cyc_adj(verts.size());
    vector<int> degree(verts.size(), 0);
    auto get_idx = [&](int v) {
        return (int)(lower_bound(verts.begin(), verts.end(), v) - verts.begin());
    };
    auto add_dir = [&](int from, int to, int edge_id) {
        int idx = get_idx(from);
        cyc_adj[idx][degree[idx]++] = {to, edge_id};
    };

    for (int e : cyc) {
        add_dir(U[e], V[e], e);
        add_dir(V[e], U[e], e);
    }

    auto walk = [&](pair<int, int> first_step, vector<int>& path) {
        int prev = start;
        int curr = first_step.first;
        int edge_id = first_step.second;
        path.push_back(edge_id);
        while (curr != end) {
            int idx = get_idx(curr);
            pair<int, int> next_step = cyc_adj[idx][0];
            if (next_step.first == prev) next_step = cyc_adj[idx][1];
            path.push_back(next_step.second);
            prev = curr;
            curr = next_step.first;
        }
    };

    int start_idx = get_idx(start);
    walk(cyc_adj[start_idx][0], pathA);
    walk(cyc_adj[start_idx][1], pathB);
}

// Check if two edges have conflicting closure requirements
bool conflict(int g1, int g2) {
    if (is_bot[g1] || is_bot[g2]) return true;
    for (auto r1 : group_req[g1]) {
        for (auto r2 : group_req[g2]) {
            if (r1.first == r2.first && r1.second != r2.second) return true;
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    DSU dsu(m);
    for (int i = 1; i <= m; ++i) {
        int u, v, o;
        cin >> u >> v >> o;
        adj[u].push_back({u, v, i});
        adj[v].push_back({v, u, i});
        U[i] = u; V[i] = v;
        dsu.unite(i, o);
    }
    
    for (int i = 1; i <= m; ++i) link_group[i] = dsu.find(i);

    dfs_iterative(1);

    for (int i = 1; i <= m; ++i) {
        if (edge_to_cycle[i] == 0) {
            bridges.push_back(i);
            edge_to_bridge[i] = bridges.size();
        }
    }

    int C = cycles.size();
    int B = bridges.size();

    // Construct Block-Cut Tree
    for (int i = 0; i < C; ++i) {
        int bct_node = n + 1 + i;
        vector<int> verts;
        verts.reserve(cycles[i].size());
        for (int e : cycles[i]) {
            verts.push_back(U[e]);
            verts.push_back(V[e]);
        }
        sort(verts.begin(), verts.end());
        verts.erase(unique(verts.begin(), verts.end()), verts.end());
        for (int v : verts) {
            bct_adj[v].push_back(bct_node);
            bct_adj[bct_node].push_back(v);
        }
    }
    for (int i = 0; i < B; ++i) {
        int bct_node = n + C + 1 + i;
        int e = bridges[i];
        bct_adj[U[e]].push_back(bct_node); bct_adj[bct_node].push_back(U[e]);
        bct_adj[V[e]].push_back(bct_node); bct_adj[bct_node].push_back(V[e]);
    }

    vector<int> path2 = get_bct_path(1, 2);
    vector<int> path3 = get_bct_path(1, 3);

    if (path2.empty()) { cout << "Nej\n"; return 0; }
    if (path3.empty()) { cout << "Ja\n"; return 0; }

    // Phase 1: Record requirements from the 1 -> 2 path
    for (size_t i = 1; i < path2.size(); i += 2) {
        int block = path2[i];
        int entry = path2[i - 1];
        int exit = path2[i + 1];
        
        if (block > n + C) { // Bridge
            int b_id = block - (n + C);
            is_bot[link_group[bridges[b_id - 1]]] = true;
        } else { // Cycle
            int c_id = block - n;
            vector<int> pA, pB;
            extract_cycle_paths(c_id, entry, exit, pA, pB);
            
            for (int e : pA) {
                int g = link_group[e];
                bool found = false;
                for (auto r : group_req[g]) if (r.first == c_id && r.second == 0) { is_bot[g] = true; found = true; break; }
                if (!found) group_req[g].push_back({c_id, 1});
            }
            for (int e : pB) {
                int g = link_group[e];
                bool found = false;
                for (auto r : group_req[g]) if (r.first == c_id && r.second == 1) { is_bot[g] = true; found = true; break; }
                if (!found) group_req[g].push_back({c_id, 0});
            }
        }
    }

    // Phase 2: Verify if any block on the 1 -> 3 path can be completely closed
    bool can_block = false;
    for (size_t i = 1; i < path3.size(); i += 2) {
        int block = path3[i];
        int entry = path3[i - 1];
        int exit = path3[i + 1];

        if (block > n + C) { // Bridge
            int b_id = block - (n + C);
            if (!is_bot[link_group[bridges[b_id - 1]]]) {
                can_block = true;
                break;
            }
        } else { // Cycle
            int c_id = block - n;
            vector<int> pA, pB;
            extract_cycle_paths(c_id, entry, exit, pA, pB);
            
            bool found_pair = false;
            for (int e1 : pA) {
                if (is_bot[link_group[e1]]) continue;
                for (int e2 : pB) {
                    if (is_bot[link_group[e2]]) continue;
                    if (!conflict(link_group[e1], link_group[e2])) {
                        found_pair = true;
                        break;
                    }
                }
                if (found_pair) break;
            }
            if (found_pair) {
                can_block = true;
                break;
            }
        }
    }

    if (can_block) cout << "Ja\n";
    else cout << "Nej\n";

    return 0;
}
