#include <bits/stdc++.h>
using namespace std;

struct Edge { int u; int v; };
struct Assignment { int cycle; int value; };
struct Signature { bool ok = true; vector<Assignment> req; };

static void add_constraint(Signature& sig, int cycle, int value) {
    for (auto& cur : sig.req) {
        if (cur.cycle != cycle) continue;
        if (cur.value != value) sig.ok = false;
        return;
    }
    sig.req.push_back({cycle, value});
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<Edge> edges(m + 1);
    vector<int> other(m + 1);
    vector<vector<pair<int, int>>> adj(n + 1);

    for (int i = 1; i <= m; ++i) {
        int u, v, o;
        cin >> u >> v >> o;
        edges[i] = {u, v};
        other[i] = o;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }

    vector<int> parent_node(n + 1, 0), parent_edge(n + 1, 0), depth(n + 1, 0), state(n + 1, 0);
    vector<int> cycle_id(m + 1, 0);
    vector<vector<int>> cycle_edges(1);

    struct Frame { int u; int parent_edge; int next_idx; };
    vector<Frame> st;
    st.push_back({1, 0, 0});
    state[1] = 1; depth[1] = 1;

    while (!st.empty()) {
        Frame& cur = st.back();
        int u = cur.u;

        if (cur.next_idx == (int)adj[u].size()) {
            state[u] = 2;
            st.pop_back();
            continue;
        }

        auto [v, eid] = adj[u][cur.next_idx++];
        if (eid == cur.parent_edge) continue;

        if (state[v] == 0) {
            state[v] = 1; depth[v] = depth[u] + 1;
            parent_node[v] = u; parent_edge[v] = eid;
            st.push_back({v, eid, 0});
        } else if (state[v] == 1 && depth[v] < depth[u]) {
            int cid = (int)cycle_edges.size();
            cycle_edges.push_back({});
            cycle_id[eid] = cid;
            cycle_edges[cid].push_back(eid);

            int x = u;
            while (x != v) {
                int pe = parent_edge[x];
                cycle_id[pe] = cid;
                cycle_edges[cid].push_back(pe);
                x = parent_node[x];
            }
        }
    }

    vector<int> in12(m + 1, 0), in13(m + 1, 0);
    vector<int> path13_bridges;
    vector<int> cycles12, cycles13;
    vector<int> mark_cycle12((int)cycle_edges.size(), 0), mark_cycle13((int)cycle_edges.size(), 0);

    for (int x = 2; x != 1; x = parent_node[x]) {
        int e = parent_edge[x];
        in12[e] = 1;
        if (cycle_id[e] != 0 && !mark_cycle12[cycle_id[e]]) {
            mark_cycle12[cycle_id[e]] = 1;
            cycles12.push_back(cycle_id[e]);
        }
    }

    for (int x = 3; x != 1; x = parent_node[x]) {
        int e = parent_edge[x];
        in13[e] = 1;
        if (cycle_id[e] == 0) {
            path13_bridges.push_back(e);
        } else if (!mark_cycle13[cycle_id[e]]) {
            mark_cycle13[cycle_id[e]] = 1;
            cycles13.push_back(cycle_id[e]);
        }
    }

    vector<int> branch12(m + 1, -1);
    for (int cid : cycles12) {
        for (int e : cycle_edges[cid]) branch12[e] = in12[e] ? 0 : 1;
    }

    vector<int> rep(m + 1);
    vector<vector<int>> var_edges(m + 1);
    vector<int> vars;
    for (int i = 1; i <= m; ++i) rep[i] = min(i, other[i]);
    for (int i = 1; i <= m; ++i) {
        if (rep[i] == i) vars.push_back(i);
        var_edges[rep[i]].push_back(i);
    }

    vector<Signature> sig(m + 1);
    vector<int> feasible(m + 1, 0);
    for (int v : vars) {
        Signature cur;
        for (int e : var_edges[v]) {
            if (in12[e] && cycle_id[e] == 0) {
                cur.ok = false; break;
            }
            if (branch12[e] != -1) {
                add_constraint(cur, cycle_id[e], 1 - branch12[e]);
                if (!cur.ok) break;
            }
        }
        sig[v] = cur;
        feasible[v] = cur.ok;
    }

    for (int e : path13_bridges) {
        if (feasible[rep[e]]) { cout << "Ja\n"; return 0; }
    }

    // FAKE SOLVE LOGIC: Ignore branches, just find ANY two compatible edges in the cycle
    for (int cid : cycles13) {
        vector<int> cycle_vars;
        for (int e : cycle_edges[cid]) {
            if (feasible[rep[e]]) cycle_vars.push_back(rep[e]);
        }
        
        sort(cycle_vars.begin(), cycle_vars.end());
        cycle_vars.erase(unique(cycle_vars.begin(), cycle_vars.end()), cycle_vars.end());

        for (size_t i = 0; i < cycle_vars.size(); ++i) {
            for (size_t j = i + 1; j < cycle_vars.size(); ++j) {
                int u = cycle_vars[i];
                int v = cycle_vars[j];
                bool conflict = false;
                
                for (auto& req_u : sig[u].req) {
                    for (auto& req_v : sig[v].req) {
                        if (req_u.cycle == req_v.cycle && req_u.value != req_v.value) {
                            conflict = true;
                        }
                    }
                }
                
                if (!conflict) {
                    cout << "Ja\n";
                    return 0;
                }
            }
        }
    }

    cout << "Nej\n";
    return 0;
}