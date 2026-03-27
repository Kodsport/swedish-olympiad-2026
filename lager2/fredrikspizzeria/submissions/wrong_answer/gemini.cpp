#include <bits/stdc++.h>
using namespace std;

const int MAXM = 200005;
const int MAXN = 200005;

struct Edge {
    int to, id;
};

int N, M;
vector<Edge> adj[MAXN];
int link_edge[MAXM];

int dfn_node[MAXN], low_node[MAXN], timer_node = 0;
vector<int> edge_stack;
vector<vector<int>> cycles;
vector<int> bridges;
int comp_of_edge[MAXM];

void dfs_bcc(int u, int p_edge) {
    dfn_node[u] = low_node[u] = ++timer_node;
    for (auto& edge : adj[u]) {
        int v = edge.to;
        int id = edge.id;
        if (id == p_edge) continue;
        if (dfn_node[v]) {
            low_node[u] = min(low_node[u], dfn_node[v]);
            if (dfn_node[v] < dfn_node[u]) edge_stack.push_back(id);
        } else {
            edge_stack.push_back(id);
            dfs_bcc(v, id);
            low_node[u] = min(low_node[u], low_node[v]);
            if (low_node[v] >= dfn_node[u]) {
                vector<int> comp;
                while (true) {
                    int e = edge_stack.back();
                    edge_stack.pop_back();
                    comp.push_back(e);
                    if (e == id) break;
                }
                if (comp.size() == 1) bridges.push_back(comp[0]);
                else cycles.push_back(comp);
            }
        }
    }
}

// Lösningens kärnstrukturer för Block-Cut Tree
vector<int> tree_adj[MAXN + MAXM];
int edge_u[MAXM], edge_v[MAXM];

vector<int> get_path(int S, int T) {
    vector<int> parent(N + cycles.size() + 1, 0);
    queue<int> q;
    q.push(S);
    parent[S] = -1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == T) break;
        for (int v : tree_adj[u]) {
            if (!parent[v]) {
                parent[v] = u;
                q.push(v);
            }
        }
    }
    vector<int> path;
    if (!parent[T]) return path;
    for (int curr = T; curr != -1; curr = parent[curr]) path.push_back(curr);
    reverse(path.begin(), path.end());
    return path;
}

vector<int> P_cycle_0[MAXM], P_cycle_1[MAXM];
vector<int> E12, E13;
struct CyclePaths { vector<int> p0, p1; };
vector<CyclePaths> C13;
int cycle_idx_12 = 0;

int scc[MAXM], scc_timer = 0, scc_cnt = 0;
int dfn_scc[MAXM], low_scc[MAXM];
bool in_stack[MAXM];
stack<int> st_scc;

void dfs_scc(int u) {
    dfn_scc[u] = low_scc[u] = ++scc_timer;
    st_scc.push(u);
    in_stack[u] = true;
    int v = link_edge[u];
    if (!dfn_scc[v]) {
        dfs_scc(v);
        low_scc[u] = min(low_scc[u], low_scc[v]);
    } else if (in_stack[v]) {
        low_scc[u] = min(low_scc[u], dfn_scc[v]);
    }
    if (low_scc[u] == dfn_scc[u]) {
        scc_cnt++;
        while (true) {
            int w = st_scc.top();
            st_scc.pop();
            in_stack[w] = false;
            scc[w] = scc_cnt;
            if (w == u) break;
        }
    }
}

// Persistent segment tree
const int MAX_NODES = 20000000;
int ls_node[MAX_NODES], rs_node[MAX_NODES], sz[MAX_NODES];
int node_cnt = 0;

int insert_seg(int l, int r, int pos) {
    int n = ++node_cnt;
    sz[n] = 1;
    if (l == r) return n;
    int mid = (l + r) / 2;
    if (pos <= mid) ls_node[n] = insert_seg(l, mid, pos);
    else rs_node[n] = insert_seg(mid + 1, r, pos);
    return n;
}

int merge_seg(int n1, int n2) {
    if (!n1 || !n2) return n1 ? n1 : n2;
    int n = ++node_cnt;
    ls_node[n] = merge_seg(ls_node[n1], ls_node[n2]);
    rs_node[n] = merge_seg(rs_node[n1], rs_node[n2]);
    sz[n] = sz[ls_node[n]] + sz[rs_node[n]];
    return n;
}

bool intersect(int n1, int n2) {
    if (!n1 || !n2) return false;
    if (!ls_node[n1] && !rs_node[n1]) return true; 
    return intersect(ls_node[n1], ls_node[n2]) || intersect(rs_node[n1], rs_node[n2]);
}

vector<int> rev_scc[MAXM];
int root0[MAXM], root1[MAXM];
int e12_cnt[MAXM];
bool is_forced[MAXM];

void dfs_rev(int u) {
    is_forced[u] = (e12_cnt[u] > 0);
    for (int v : rev_scc[u]) {
        dfs_rev(v);
        root0[u] = merge_seg(root0[u], root0[v]);
        root1[u] = merge_seg(root1[u], root1[v]);
        e12_cnt[u] += e12_cnt[v];
        is_forced[u] |= is_forced[v];
    }
    if (e12_cnt[u] > 0) is_forced[u] = true;
    if (!is_forced[u] && intersect(root0[u], root1[u])) {
        is_forced[u] = true;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M)) return 0;
    for (int i = 1; i <= M; i++) {
        int u, v, o;
        cin >> u >> v >> o;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
        link_edge[i] = o;
        edge_u[i] = u;
        edge_v[i] = v;
    }

    dfs_bcc(1, 0);

    for (int e : bridges) {
        comp_of_edge[e] = e;
        tree_adj[edge_u[e]].push_back(N + e);
        tree_adj[N + e].push_back(edge_u[e]);
        tree_adj[edge_v[e]].push_back(N + e);
        tree_adj[N + e].push_back(edge_v[e]);
    }
    
    for (int i = 0; i < cycles.size(); i++) {
        int c_id = N + M + 1 + i;
        for (int e : cycles[i]) {
            comp_of_edge[e] = c_id;
            tree_adj[edge_u[e]].push_back(c_id);
            tree_adj[c_id].push_back(edge_u[e]);
            tree_adj[edge_v[e]].push_back(c_id);
            tree_adj[c_id].push_back(edge_v[e]);
        }
        // Ta bort dubbletter i adjacenslistan för c_id pga att c_id fästs till hörn
        sort(tree_adj[c_id].begin(), tree_adj[c_id].end());
        tree_adj[c_id].erase(unique(tree_adj[c_id].begin(), tree_adj[c_id].end()), tree_adj[c_id].end());
    }

    auto extract_path = [&](int S, int T, vector<int>& E, vector<CyclePaths>& C) {
        vector<int> p = get_path(S, T);
        for (int i = 1; i + 1 < p.size(); i += 2) {
            int comp = p[i];
            if (comp >= N + M + 1) { // Cykel
                int in_node = p[i - 1], out_node = p[i + 1];
                int c_idx = comp - N - M - 1;
                vector<int> edges = cycles[c_idx];
                vector<vector<pair<int, int>>> cadj(N + 1);
                for (int e : edges) {
                    cadj[edge_u[e]].push_back({edge_v[e], e});
                    cadj[edge_v[e]].push_back({edge_u[e], e});
                }
                
                CyclePaths cp;
                for (int iter = 0; iter < 2; iter++) {
                    int nxt = cadj[in_node][iter].first;
                    int cur_e = cadj[in_node][iter].second;
                    vector<int> way;
                    way.push_back(cur_e);
                    int curr = nxt, prev = in_node;
                    while (curr != out_node) {
                        for (auto& edge : cadj[curr]) {
                            if (edge.first != prev) {
                                way.push_back(edge.second);
                                prev = curr;
                                curr = edge.first;
                                break;
                            }
                        }
                    }
                    if (iter == 0) cp.p0 = way;
                    else cp.p1 = way;
                }
                C.push_back(cp);
            } else { // Bro
                E.push_back(comp - N);
            }
        }
    };

    vector<CyclePaths> C12;
    extract_path(1, 2, E12, C12);
    extract_path(1, 3, E13, C13);

    for (int i = 1; i <= M; i++) if (!dfn_scc[i]) dfs_scc(i);

    for (int e : E12) e12_cnt[scc[e]]++;
    for (auto& cp : C12) {
        cycle_idx_12++;
        for (int e : cp.p0) root0[scc[e]] = merge_seg(root0[scc[e]], insert_seg(1, M, cycle_idx_12));
        for (int e : cp.p1) root1[scc[e]] = merge_seg(root1[scc[e]], insert_seg(1, M, cycle_idx_12));
    }

    int in_degree[MAXM] = {0};
    for (int i = 1; i <= M; i++) {
        int u = scc[i], v = scc[link_edge[i]];
        if (u != v) {
            rev_scc[v].push_back(u); // Omvänd kant: förälder till barn i scc_tree
            in_degree[u]++;
        }
    }
    // Avduplicera
    for(int i=1; i<=scc_cnt; ++i) {
        sort(rev_scc[i].begin(), rev_scc[i].end());
        rev_scc[i].erase(unique(rev_scc[i].begin(), rev_scc[i].end()), rev_scc[i].end());
    }

    for (int i = 1; i <= scc_cnt; i++) {
        if (in_degree[i] == 0) dfs_rev(i);
    }

    bool possible = false;
    for (int e : E13) {
        if (!is_forced[scc[e]]) { possible = true; break; }
    }

    if (!possible) {
        for (auto& cp : C13) {
            vector<int> A = cp.p0;
            vector<int> B = cp.p1;
            auto cmp = [&](int a, int b) { return sz[root0[scc[a]]] + sz[root1[scc[a]]] < sz[root0[scc[b]]] + sz[root1[scc[b]]]; };
            sort(A.begin(), A.end(), cmp);
            sort(B.begin(), B.end(), cmp);
            
            bool found = false;
            int ops = 0;
            for (int a : A) {
                if (is_forced[scc[a]]) continue;
                for (int b : B) {
                    if (is_forced[scc[b]]) continue;
                    ops++;
                    if (!intersect(root0[scc[a]], root1[scc[b]]) && !intersect(root1[scc[a]], root0[scc[b]])) {
                        found = true; break;
                    }
                    if (ops > 2000) break; // Heuristik limit
                }
                if (found || ops > 2000) break;
            }
            if (found) { possible = true; break; }
        }
    }

    if (possible) cout << "Ja\n";
    else cout << "Nej\n";

    return 0;
}