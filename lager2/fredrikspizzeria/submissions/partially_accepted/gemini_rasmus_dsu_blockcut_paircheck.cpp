#include <iostream>
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
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
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

// DFS to identify cycles and bridges in the Cactus graph
void dfs(int u, int p, int p_edge) {
    vis[u] = true;
    depth[u] = depth[p] + 1;
    pa[u] = p;
    pa_edge[u] = p_edge;
    for (const auto& edge : adj[u]) {
        int v = edge.v;
        int id = edge.id;
        if (v == p) continue;
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
        } else {
            dfs(v, u, id);
        }
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
    vector<vector<pair<int, int>>> cyc_adj(MAXM);
    for (int e : cycles[c_id - 1]) {
        cyc_adj[U[e]].push_back({V[e], e});
        cyc_adj[V[e]].push_back({U[e], e});
    }
    
    int curr = start, prev = 0;
    pathA.push_back(cyc_adj[curr][0].second);
    curr = cyc_adj[curr][0].first;
    while (curr != end) {
        for (auto& edge : cyc_adj[curr]) {
            if (edge.first != start && edge.second != pathA.back()) {
                pathA.push_back(edge.second);
                curr = edge.first;
                break;
            }
        }
    }
    
    curr = start;
    pathB.push_back(cyc_adj[curr][1].second);
    curr = cyc_adj[curr][1].first;
    while (curr != end) {
        for (auto& edge : cyc_adj[curr]) {
            if (edge.first != start && edge.second != pathB.back()) {
                pathB.push_back(edge.second);
                curr = edge.first;
                break;
            }
        }
    }
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

    dfs(1, 0, 0);

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
        set<int> verts;
        for (int e : cycles[i]) { verts.insert(U[e]); verts.insert(V[e]); }
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