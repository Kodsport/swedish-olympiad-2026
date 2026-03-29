#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdint>

using namespace std;

const int MAXN = 200005;
const int MAXM = 200005;

int N, M;
vector<pair<int, int>> adj[MAXN];
int U[MAXM], V_edge[MAXM], O[MAXM];

int depth[MAXN], parent_edge[MAXN], parent_node[MAXN];
int cycle_id[MAXM];
int num_cycles = 0;
vector<int> cycle_edges[MAXM];

struct DFSState { int u, p, d, edge_idx; };

void dfs_cycles_iterative(int start) {
    vector<DFSState> st;
    st.push_back({start, 0, 1, 0});
    depth[start] = 1;
    while (!st.empty()) {
        auto& curr = st.back();
        int u = curr.u;
        if (curr.edge_idx < adj[u].size()) {
            auto edge = adj[u][curr.edge_idx++];
            int v = edge.first;
            int id = edge.second;
            if (v == curr.p) continue;
            if (depth[v]) {
                if (depth[v] < depth[u]) { 
                    num_cycles++;
                    cycle_id[id] = num_cycles;
                    cycle_edges[num_cycles].push_back(id);
                    int temp = u;
                    while (temp != v) {
                        int e = parent_edge[temp];
                        cycle_id[e] = num_cycles;
                        cycle_edges[num_cycles].push_back(e);
                        temp = parent_node[temp];
                    }
                }
            } else {
                parent_edge[v] = id;
                parent_node[v] = u;
                depth[v] = depth[u] + 1;
                st.push_back({v, u, depth[v], 0});
            }
        } else {
            st.pop_back();
        }
    }
}

bool vis[MAXN];
int pre_n[MAXN], pre_e[MAXN];

bool bfs_path(int S, int T, vector<int>& p_nodes, vector<int>& p_edges) {
    queue<int> q;
    for (int i = 1; i <= N; ++i) vis[i] = false;
    q.push(S); vis[S] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == T) break;
        for (auto edge : adj[u]) {
            int v = edge.first;
            if (!vis[v]) {
                vis[v] = true;
                pre_n[v] = u;
                pre_e[v] = edge.second;
                q.push(v);
            }
        }
    }
    if (!vis[T]) return false;
    int curr = T;
    while (curr != S) {
        p_nodes.push_back(curr);
        p_edges.push_back(pre_e[curr]);
        curr = pre_n[curr];
    }
    p_nodes.push_back(S);
    reverse(p_nodes.begin(), p_nodes.end());
    reverse(p_edges.begin(), p_edges.end());
    return true;
}

// 2-SAT Constants
int K = 0;
int VARS = 0;
inline int C(int i) { return i; }
inline int notC(int i) { return i + M; }
inline int X(int k) { return 2 * M + k; }
inline int notX(int k) { return 2 * M + K + k; }

const int MAX_VARS = 800005;
vector<int> sat_adj[MAX_VARS];

void add_impl(int a, int b) {
    sat_adj[a].push_back(b);
}

int dfn[MAX_VARS], low[MAX_VARS], scc[MAX_VARS], timer = 0, scc_cnt = 0;
int tar_st[MAX_VARS], st_ptr = 0;
bool in_st[MAX_VARS];
int edge_ptr[MAX_VARS];

void tarjan() {
    vector<int> call_st;
    for (int i = 1; i <= VARS; ++i) {
        if (!dfn[i]) {
            call_st.push_back(i);
            while (!call_st.empty()) {
                int u = call_st.back();
                if (!dfn[u]) {
                    dfn[u] = low[u] = ++timer;
                    tar_st[++st_ptr] = u;
                    in_st[u] = true;
                }
                bool advanced = false;
                while (edge_ptr[u] < sat_adj[u].size()) {
                    int v = sat_adj[u][edge_ptr[u]++];
                    if (!dfn[v]) {
                        call_st.push_back(v);
                        advanced = true;
                        break;
                    } else if (in_st[v]) {
                        low[u] = min(low[u], dfn[v]);
                    }
                }
                if (advanced) continue;
                call_st.pop_back();
                if (low[u] == dfn[u]) {
                    scc_cnt++;
                    while (true) {
                        int v = tar_st[st_ptr--];
                        in_st[v] = false;
                        scc[v] = scc_cnt;
                        if (u == v) break;
                    }
                }
                if (!call_st.empty()) {
                    int p = call_st.back();
                    low[p] = min(low[p], low[u]);
                }
            }
        }
    }
}

vector<int> dag[MAX_VARS];

struct BS256 {
    uint64_t d[4];
    inline void clear() { d[0] = d[1] = d[2] = d[3] = 0; }
    inline void set(int i) { d[i >> 6] |= (1ULL << (i & 63)); }
    inline void OR(const BS256& o) {
        d[0] |= o.d[0]; d[1] |= o.d[1];
        d[2] |= o.d[2]; d[3] |= o.d[3];
    }
    inline bool has_zero(int n) const {
        for (int i = 0; i < n; ++i) {
            if (!(d[i >> 6] & (1ULL << (i & 63)))) return true;
        }
        return false;
    }
};

BS256 reach[MAX_VARS];
bool cycle_on_sp12[MAXM];
int cycle_mapped_id[MAXM];
bool in_sp12_edge[MAXM], in_sp13_edge[MAXM];
bool local_valid[MAXM];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M)) return 0;
    for (int i = 1; i <= M; ++i) {
        cin >> U[i] >> V_edge[i] >> O[i];
        adj[U[i]].push_back({V_edge[i], i});
        adj[V_edge[i]].push_back({U[i], i});
    }

    dfs_cycles_iterative(1);

    vector<int> sp12_nodes, sp12_edges, sp13_nodes, sp13_edges;
    bfs_path(1, 2, sp12_nodes, sp12_edges);
    bfs_path(1, 3, sp13_nodes, sp13_edges);

    for (int e : sp12_edges) {
        in_sp12_edge[e] = true;
        if (cycle_id[e] != 0) cycle_on_sp12[cycle_id[e]] = true;
    }
    for (int e : sp13_edges) in_sp13_edge[e] = true;

    for (int i = 1; i <= num_cycles; ++i) {
        if (cycle_on_sp12[i]) cycle_mapped_id[i] = ++K;
    }

    VARS = 2 * M + 2 * K;

    for (int i = 1; i <= M; ++i) {
        add_impl(C(i), C(O[i]));
        add_impl(notC(O[i]), notC(i));
    }

    for (int e : sp12_edges) {
        if (cycle_id[e] == 0) add_impl(C(e), notC(e)); 
    }

    for (int i = 1; i <= M; ++i) {
        int c = cycle_id[i];
        if (c != 0 && cycle_on_sp12[c]) {
            int k = cycle_mapped_id[c];
            if (in_sp12_edge[i]) {
                add_impl(X(k), notC(i));
                add_impl(C(i), notX(k));
            } else {
                add_impl(C(i), X(k));
                add_impl(notX(k), notC(i));
            }
        }
    }

    tarjan();

    for (int u = 1; u <= VARS; ++u) {
        for (int v : sat_adj[u]) {
            if (scc[u] != scc[v]) {
                dag[scc[u]].push_back(scc[v]);
            }
        }
    }
    for (int i = 1; i <= scc_cnt; ++i) {
        sort(dag[i].begin(), dag[i].end());
        dag[i].erase(unique(dag[i].begin(), dag[i].end()), dag[i].end());
    }

    // Phase 1: Local Validity Check
    vector<pair<int, int>> local_q;
    for (int e : sp13_edges) {
        if (cycle_id[e] == 0) local_q.push_back({C(e), notC(e)});
    }
    for (int i = 1; i <= num_cycles; ++i) {
        if (cycle_on_sp12[i]) continue;
        bool on_sp13 = false;
        for (int e : cycle_edges[i]) if (in_sp13_edge[e]) on_sp13 = true;
        if (on_sp13) {
            for (int e : cycle_edges[i]) local_q.push_back({C(e), notC(e)});
        }
    }

    for (int start = 0; start < local_q.size(); start += 256) {
        int chunk_sz = min(256, (int)local_q.size() - start);
        for (int i = 1; i <= scc_cnt; ++i) reach[i].clear();
        for (int i = 0; i < chunk_sz; ++i) {
            reach[scc[local_q[start + i].second]].set(i);
        }
        for (int i = 1; i <= scc_cnt; ++i) {
            for (int v : dag[i]) reach[i].OR(reach[v]);
        }
        for (int i = 0; i < chunk_sz; ++i) {
            int u = local_q[start + i].first;
            int bit = i;
            if (!(reach[scc[u]].d[bit >> 6] & (1ULL << (bit & 63)))) {
                local_valid[u] = true; 
            }
        }
    }

    for (int e : sp13_edges) {
        if (cycle_id[e] == 0 && local_valid[C(e)]) {
            cout << "Ja\n"; return 0;
        }
    }

    // Phase 2: Cross Validation for 1-3 Cycles
    bool cycle_on_sp13[MAXM] = {false};
    for (int e : sp13_edges) if (cycle_id[e] != 0) cycle_on_sp13[cycle_id[e]] = true;

    for (int i = 1; i <= num_cycles; ++i) {
        if (cycle_on_sp13[i]) {
            vector<int> L, R;
            for (int e : cycle_edges[i]) {
                if (local_valid[C(e)]) {
                    if (in_sp13_edge[e]) L.push_back(C(e));
                    else R.push_back(C(e));
                }
            }
            if (L.empty() || R.empty()) continue;

            for (int start = 0; start < R.size(); start += 256) {
                int chunk_sz = min(256, (int)R.size() - start);
                for (int j = 1; j <= scc_cnt; ++j) reach[j].clear();
                for (int j = 0; j < chunk_sz; ++j) {
                    reach[scc[notC(R[start + j])]].set(j);
                }
                for (int j = 1; j <= scc_cnt; ++j) {
                    for (int v : dag[j]) reach[j].OR(reach[v]);
                }
                for (int u : L) {
                    if (reach[scc[u]].has_zero(chunk_sz)) {
                        cout << "Ja\n"; return 0;
                    }
                }
            }
        }
    }

    cout << "Nej\n";
    return 0;
}
