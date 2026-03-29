#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 200005;
const int MAXM = 200005;
const int MAXBCT = 400005;

// ---- Disjoint Set Union (DSU) ----
int parent_dsu[MAXM];
int find_dsu(int i) {
    if (parent_dsu[i] == i) return i;
    return parent_dsu[i] = find_dsu(parent_dsu[i]);
}
void unite_dsu(int i, int j) {
    int root_i = find_dsu(i);
    int root_j = find_dsu(j);
    if (root_i != root_j) parent_dsu[root_i] = root_j;
}
int link_group[MAXM];

// ---- Chain Forward Star Graph ----
int head[MAXN], to_edge[MAXM * 2], nxt[MAXM * 2], edge_id[MAXM * 2];
int edge_cnt = 0;
void add_edge(int u, int v, int id) {
    edge_cnt++;
    to_edge[edge_cnt] = v;
    edge_id[edge_cnt] = id;
    nxt[edge_cnt] = head[u];
    head[u] = edge_cnt;
}

int U[MAXM], V[MAXM];

// ---- DFS & Cycle Detection ----
bool vis[MAXN];
int depth[MAXN], pa_node[MAXN], pa_edge[MAXN];
vector<int> cycles[MAXM]; 
int edge_cycle_id[MAXM];
int num_cycles = 0;

void dfs(int u, int p, int p_edge) {
    vis[u] = true;
    depth[u] = depth[p] + 1;
    pa_node[u] = p;
    pa_edge[u] = p_edge;

    for (int e = head[u]; e != 0; e = nxt[e]) {
        int v = to_edge[e];
        int id = edge_id[e];
        if (v == p) continue;

        if (vis[v]) {
            if (depth[v] < depth[u]) { 
                num_cycles++;
                cycles[num_cycles].push_back(id);
                edge_cycle_id[id] = num_cycles;
                int curr = u;
                while (curr != v) {
                    int ce = pa_edge[curr];
                    cycles[num_cycles].push_back(ce);
                    edge_cycle_id[ce] = num_cycles;
                    curr = pa_node[curr];
                }
            }
        } else {
            dfs(v, u, id);
        }
    }
}

// ---- Block-Cut Tree (BCT) ----
int bct_head[MAXBCT], bct_to[MAXBCT * 2], bct_nxt[MAXBCT * 2], bct_edge_id[MAXBCT * 2];
int bct_edge_cnt = 0;
void add_bct_edge(int u, int v, int id) {
    bct_edge_cnt++;
    bct_to[bct_edge_cnt] = v;
    bct_edge_id[bct_edge_cnt] = id;
    bct_nxt[bct_edge_cnt] = bct_head[u];
    bct_head[u] = bct_edge_cnt;
    
    bct_edge_cnt++;
    bct_to[bct_edge_cnt] = u;
    bct_edge_id[bct_edge_cnt] = id;
    bct_nxt[bct_edge_cnt] = bct_head[v];
    bct_head[v] = bct_edge_cnt;
}

bool bct_vis[MAXBCT];
int bct_pa_node[MAXBCT], bct_pa_edge[MAXBCT];

vector<int> get_bct_path(int target) {
    vector<int> path;
    if (!bct_vis[target]) return path;
    int curr = target;
    while (curr != 0) {
        path.push_back(curr);
        curr = bct_pa_node[curr];
    }
    reverse(path.begin(), path.end());
    return path;
}

// ---- Block & Cycle Extraction ----
struct Block {
    int type; // 0 for bridge, 1 for cycle
    int id, entry, exit;
};

vector<Block> extract_blocks(const vector<int>& path, int N) {
    vector<Block> blocks;
    for (size_t i = 0; i + 1 < path.size(); ) {
        int u = path[i];
        int next_node = path[i+1];
        if (next_node > N) {
            blocks.push_back({1, next_node - N, u, path[i+2]});
            i += 2;
        } else {
            int b_id = (bct_pa_node[next_node] == u) ? bct_pa_edge[next_node] : bct_pa_edge[u];
            blocks.push_back({0, b_id, u, next_node});
            i += 1;
        }
    }
    return blocks;
}

struct CycEdge { int to, id; };
vector<CycEdge> local_adj[MAXN];

void extract_cycle_sides(int c, int u, int v, vector<int>& side_a, vector<int>& side_b) {
    vector<int> touched;
    for (int id : cycles[c]) {
        local_adj[U[id]].push_back({V[id], id});
        local_adj[V[id]].push_back({U[id], id});
        touched.push_back(U[id]); touched.push_back(V[id]);
    }

    auto trace_side = [&](int start_edge, vector<int>& side) {
        int curr = (local_adj[u][start_edge].to);
        int prev_node = u;
        side.push_back(local_adj[u][start_edge].id);
        while (curr != v) {
            for (auto& edge : local_adj[curr]) {
                if (edge.to != prev_node) {
                    side.push_back(edge.id);
                    prev_node = curr;
                    curr = edge.to;
                    break;
                }
            }
        }
    };

    trace_side(0, side_a);
    trace_side(1, side_b);

    for (int t : touched) local_adj[t].clear();
}

// ---- 2-SAT Conflict Resolution ----
bool is_bot[MAXM];
vector<pair<int, int>> req[MAXM];

bool conflict(int g1, int g2) {
    if (is_bot[g1] || is_bot[g2]) return true;
    size_t i = 0, j = 0;
    while (i < req[g1].size() && j < req[g2].size()) {
        if (req[g1][i].first == req[g2][j].first) {
            if (req[g1][i].second != req[g2][j].second) return true;
            i++; j++;
        } else if (req[g1][i].first < req[g2][j].first) {
            i++;
        } else {
            j++;
        }
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    for (int i = 1; i <= M; ++i) parent_dsu[i] = i;

    for (int i = 1; i <= M; ++i) {
        int u, v, o;
        cin >> u >> v >> o;
        U[i] = u; V[i] = v;
        add_edge(u, v, i);
        add_edge(v, u, i);
        unite_dsu(i, o);
    }

    for (int i = 1; i <= M; ++i) link_group[i] = find_dsu(i);

    // 1. Build Base Paths & Extract Cycles
    dfs(1, 0, 0);

    // 2. Build BCT (Block-Cut Tree) Array representation
    for (int i = 1; i <= M; ++i) {
        if (edge_cycle_id[i] == 0) add_bct_edge(U[i], V[i], i);
    }
    for (int c = 1; c <= num_cycles; ++c) {
        vector<int> cyc_verts;
        for (int id : cycles[c]) { cyc_verts.push_back(U[id]); cyc_verts.push_back(V[id]); }
        sort(cyc_verts.begin(), cyc_verts.end());
        cyc_verts.erase(unique(cyc_verts.begin(), cyc_verts.end()), cyc_verts.end());
        for (int v : cyc_verts) add_bct_edge(v, N + c, 0);
    }

    // 3. BFS through BCT to find the unique macro-paths
    vector<int> q; q.reserve(N + num_cycles + 1);
    q.push_back(1);
    bct_vis[1] = true;
    bct_pa_node[1] = 0;

    int head_q = 0;
    while (head_q < q.size()) {
        int u = q[head_q++];
        for (int e = bct_head[u]; e != 0; e = bct_nxt[e]) {
            int v = bct_to[e];
            if (!bct_vis[v]) {
                bct_vis[v] = true;
                bct_pa_node[v] = u;
                bct_pa_edge[v] = bct_edge_id[e];
                q.push_back(v);
            }
        }
    }

    vector<int> p2 = get_bct_path(2);
    vector<int> p3 = get_bct_path(3);

    if (p2.empty()) { cout << "Nej\n"; return 0; }
    if (p3.empty()) { cout << "Ja\n"; return 0; }

    vector<Block> blocks2 = extract_blocks(p2, N);
    vector<Block> blocks3 = extract_blocks(p3, N);

    // 4. Record forced states from the Mandatory Path (1 -> 2)
    for (auto& b : blocks2) {
        if (b.type == 0) { 
            is_bot[link_group[b.id]] = true;
        } else {           
            vector<int> side_a, side_b;
            extract_cycle_sides(b.id, b.entry, b.exit, side_a, side_b);
            for (int e : side_a) req[link_group[e]].push_back({b.id, 1});
            for (int e : side_b) req[link_group[e]].push_back({b.id, 0});
        }
    }

    // Process & deduplicate state requirements
    for (int i = 1; i <= M; ++i) {
        if (is_bot[i] || req[i].empty()) continue;
        sort(req[i].begin(), req[i].end());
        req[i].erase(unique(req[i].begin(), req[i].end()), req[i].end());
        for (size_t j = 1; j < req[i].size(); ++j) {
            if (req[i][j].first == req[i][j-1].first && req[i][j].second != req[i][j-1].second) {
                is_bot[i] = true; break;
            }
        }
        if (is_bot[i]) req[i].clear();
    }

    // 5. Evaluate valid blockades on Rat's path (1 -> 3) - PREFIX LOGIC REMOVED
    bool can_block = false;
    for (size_t i = 0; i < blocks3.size(); ++i) {
        auto& b = blocks3[i];
        if (b.type == 0) {
            if (!is_bot[link_group[b.id]]) { can_block = true; break; }
        } else {
            vector<int> side_a, side_b;
            extract_cycle_sides(b.id, b.entry, b.exit, side_a, side_b);
            bool found_pair = false;
            for (int e_a : side_a) {
                if (is_bot[link_group[e_a]]) continue;
                for (int e_b : side_b) {
                    if (is_bot[link_group[e_b]]) continue;
                    if (!conflict(link_group[e_a], link_group[e_b])) {
                        found_pair = true; break;
                    }
                }
                if (found_pair) break;
            }
            if (found_pair) { can_block = true; break; }
        }
    }

    cout << (can_block ? "Ja\n" : "Nej\n");
    return 0;
}