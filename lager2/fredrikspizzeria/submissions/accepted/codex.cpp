#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u;
    int v;
};

struct Assignment {
    int cycle;
    int value;
};

struct Signature {
    bool ok = true;
    vector<Assignment> req;
};

static void add_constraint(Signature& sig, int cycle, int value) {
    for (auto& cur : sig.req) {
        if (cur.cycle != cycle) continue;
        if (cur.value != value) sig.ok = false;
        return;
    }
    sig.req.push_back({cycle, value});
}

static uint64_t single_key(int cycle, int value) {
    return (uint64_t(cycle) << 1) | uint64_t(value);
}

static uint64_t pair_key(uint64_t a, uint64_t b) {
    if (a > b) swap(a, b);
    return (a << 32) ^ b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

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

    struct Frame {
        int u;
        int parent_edge;
        int next_idx;
    };

    vector<Frame> st;
    st.push_back({1, 0, 0});
    state[1] = 1;
    depth[1] = 1;

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
            state[v] = 1;
            depth[v] = depth[u] + 1;
            parent_node[v] = u;
            parent_edge[v] = eid;
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
    vector<int> path12_bridges;
    vector<int> path13_bridges;
    vector<int> cycles12, cycles13;
    vector<int> mark_cycle12((int)cycle_edges.size(), 0), mark_cycle13((int)cycle_edges.size(), 0);

    for (int x = 2; x != 1; x = parent_node[x]) {
        int e = parent_edge[x];
        in12[e] = 1;
        if (cycle_id[e] == 0) {
            path12_bridges.push_back(e);
        } else if (!mark_cycle12[cycle_id[e]]) {
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

    vector<int> branch12(m + 1, -1), branch13(m + 1, -1);
    for (int cid : cycles12) {
        for (int e : cycle_edges[cid]) branch12[e] = in12[e] ? 0 : 1;
    }
    for (int cid : cycles13) {
        for (int e : cycle_edges[cid]) branch13[e] = in13[e] ? 0 : 1;
    }

    vector<int> rep(m + 1);
    vector<vector<int>> var_edges(m + 1);
    vector<int> vars;
    for (int i = 1; i <= m; ++i) {
        rep[i] = min(i, other[i]);
    }
    for (int i = 1; i <= m; ++i) {
        if (rep[i] == i) vars.push_back(i);
        var_edges[rep[i]].push_back(i);
    }

    vector<Signature> sig(m + 1);
    vector<int> feasible(m + 1, 0);
    for (int v : vars) {
        Signature cur;
        sort(var_edges[v].begin(), var_edges[v].end());
        var_edges[v].erase(unique(var_edges[v].begin(), var_edges[v].end()), var_edges[v].end());

        for (int e : var_edges[v]) {
            if (in12[e] && cycle_id[e] == 0) {
                cur.ok = false;
                break;
            }
            if (branch12[e] != -1) {
                add_constraint(cur, cycle_id[e], 1 - branch12[e]);
                if (!cur.ok) break;
            }
        }

        if (cur.req.size() == 2 && cur.req[0].cycle > cur.req[1].cycle) {
            swap(cur.req[0], cur.req[1]);
        }
        sig[v] = cur;
        feasible[v] = cur.ok;
    }

    for (int e : path13_bridges) {
        if (feasible[rep[e]]) {
            cout << "Ja\n";
            return 0;
        }
    }

    for (int cid : cycles13) {
        vector<int> a_vars;
        vector<int> b_vars;

        for (int e : cycle_edges[cid]) {
            int v = rep[e];
            if (branch13[e] == 0) {
                a_vars.push_back(v);
            } else {
                b_vars.push_back(v);
            }
        }

        sort(a_vars.begin(), a_vars.end());
        a_vars.erase(unique(a_vars.begin(), a_vars.end()), a_vars.end());
        sort(b_vars.begin(), b_vars.end());
        b_vars.erase(unique(b_vars.begin(), b_vars.end()), b_vars.end());

        int total_b = 0;
        unordered_map<uint64_t, int> bad_single_b;
        unordered_map<uint64_t, int> bad_double_b;
        bad_single_b.reserve(b_vars.size() * 3 + 1);
        bad_double_b.reserve(b_vars.size() + 1);

        for (int v : b_vars) {
            if (!feasible[v]) continue;
            ++total_b;

            if (sig[v].req.size() >= 1) {
                uint64_t k0 = single_key(sig[v].req[0].cycle, sig[v].req[0].value);
                ++bad_single_b[k0];
            }
            if (sig[v].req.size() == 2) {
                uint64_t k0 = single_key(sig[v].req[0].cycle, sig[v].req[0].value);
                uint64_t k1 = single_key(sig[v].req[1].cycle, sig[v].req[1].value);
                ++bad_single_b[k1];
                ++bad_double_b[pair_key(k0, k1)];
            }
        }

        for (int v : a_vars) {
            if (!feasible[v]) continue;

            const auto& req = sig[v].req;
            if (req.empty()) {
                if (total_b > 0) {
                    cout << "Ja\n";
                    return 0;
                }
                continue;
            }

            if (req.size() == 1) {
                uint64_t bad = single_key(req[0].cycle, 1 - req[0].value);
                int incompatible = 0;
                auto it = bad_single_b.find(bad);
                if (it != bad_single_b.end()) incompatible = it->second;
                if (incompatible < total_b) {
                    cout << "Ja\n";
                    return 0;
                }
                continue;
            }

            uint64_t bad0 = single_key(req[0].cycle, 1 - req[0].value);
            uint64_t bad1 = single_key(req[1].cycle, 1 - req[1].value);

            int incompatible = 0;
            auto it0 = bad_single_b.find(bad0);
            if (it0 != bad_single_b.end()) incompatible += it0->second;
            auto it1 = bad_single_b.find(bad1);
            if (it1 != bad_single_b.end()) incompatible += it1->second;
            auto it2 = bad_double_b.find(pair_key(bad0, bad1));
            if (it2 != bad_double_b.end()) incompatible -= it2->second;

            if (incompatible < total_b) {
                cout << "Ja\n";
                return 0;
            }
        }

        int total_a = 0;
        unordered_map<uint64_t, int> bad_single_a;
        unordered_map<uint64_t, int> bad_double_a;
        bad_single_a.reserve(a_vars.size() * 3 + 1);
        bad_double_a.reserve(a_vars.size() + 1);

        for (int v : a_vars) {
            if (!feasible[v]) continue;
            ++total_a;
            if (sig[v].req.size() >= 1) {
                uint64_t k0 = single_key(sig[v].req[0].cycle, sig[v].req[0].value);
                ++bad_single_a[k0];
            }
            if (sig[v].req.size() == 2) {
                uint64_t k0 = single_key(sig[v].req[0].cycle, sig[v].req[0].value);
                uint64_t k1 = single_key(sig[v].req[1].cycle, sig[v].req[1].value);
                ++bad_single_a[k1];
                ++bad_double_a[pair_key(k0, k1)];
            }
        }

        for (int v : b_vars) {
            if (!feasible[v]) continue;

            const auto& req = sig[v].req;
            if (req.empty()) {
                if (total_a > 0) {
                    cout << "Ja\n";
                    return 0;
                }
                continue;
            }

            if (req.size() == 1) {
                uint64_t bad = single_key(req[0].cycle, 1 - req[0].value);
                int incompatible = 0;
                auto it = bad_single_a.find(bad);
                if (it != bad_single_a.end()) incompatible = it->second;
                if (incompatible < total_a) {
                    cout << "Ja\n";
                    return 0;
                }
                continue;
            }

            uint64_t bad0 = single_key(req[0].cycle, 1 - req[0].value);
            uint64_t bad1 = single_key(req[1].cycle, 1 - req[1].value);

            int incompatible = 0;
            auto it0 = bad_single_a.find(bad0);
            if (it0 != bad_single_a.end()) incompatible += it0->second;
            auto it1 = bad_single_a.find(bad1);
            if (it1 != bad_single_a.end()) incompatible += it1->second;
            auto it2 = bad_double_a.find(pair_key(bad0, bad1));
            if (it2 != bad_double_a.end()) incompatible -= it2->second;

            if (incompatible < total_a) {
                cout << "Ja\n";
                return 0;
            }
        }
    }

    cout << "Nej\n";
    return 0;
}
