#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

// A Disjoint Set Union that can undo its operations.
// We DO NOT use path compression, as it destroys the tree structure 
// making rollbacks impossible. We only use union-by-size.
struct RollbackDSU {
    vector<int> parent, sz;
    struct Action {
        int u, v; // v was attached to u
    };
    vector<Action> history;

    RollbackDSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
        sz.assign(n + 1, 1);
    }

    int find(int i) {
        while (i != parent[i]) {
            i = parent[i];
        }
        return i;
    }

    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i == root_j) return false;
        
        // Union by size
        if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
        
        parent[root_j] = root_i;
        sz[root_i] += sz[root_j];
        
        // Record the action for rollback
        history.push_back({root_i, root_j});
        return true;
    }

    int get_state() {
        return history.size();
    }

    void rollback(int target_state) {
        while (history.size() > target_state) {
            auto action = history.back();
            history.pop_back();
            sz[action.u] -= sz[action.v];
            parent[action.v] = action.v;
        }
    }
};

// Global variables
vector<vector<pair<int, int>>> edge_pairs;

// Divide and Conquer over time
void solve(int l, int r, RollbackDSU& dsu) {
    if (l == r) {
        // Base case: All pairs except `l` are currently in the DSU.
        // Check if Elsa (1) can reach the Exit (2), but the Rat (3) cannot reach Elsa.
        if (dsu.find(1) == dsu.find(2) && dsu.find(1) != dsu.find(3)) {
            cout << "Ja\n";
            exit(0); // Instantly terminate the whole program
        }
        return;
    }

    int mid = l + (r - l) / 2;

    // Traverse Left Child: solve(l, mid)
    // We must temporarily add all edges from the right half [mid + 1, r]
    int state_before_left = dsu.get_state();
    for (int i = mid + 1; i <= r; ++i) {
        for (auto& edge : edge_pairs[i]) {
            dsu.unite(edge.first, edge.second);
        }
    }
    solve(l, mid, dsu);
    dsu.rollback(state_before_left); // Undo right half

    // Traverse Right Child: solve(mid + 1, r)
    // We must temporarily add all edges from the left half [l, mid]
    int state_before_right = dsu.get_state();
    for (int i = l; i <= mid; ++i) {
        for (auto& edge : edge_pairs[i]) {
            dsu.unite(edge.first, edge.second);
        }
    }
    solve(mid + 1, r, dsu);
    dsu.rollback(state_before_right); // Undo left half
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    struct RawEdge { int u, v, link; };
    vector<RawEdge> edges(m + 1);
    
    for (int i = 1; i <= m; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].link;
    }

    vector<int> pair_id(m + 1, 0);
    int p_cnt = 0;

    // Group the linked edges into explicit pairs (so we can remove both at once)
    for (int i = 1; i <= m; ++i) {
        if (pair_id[i] == 0) {
            p_cnt++;
            pair_id[i] = p_cnt;
            pair_id[edges[i].link] = p_cnt;
        }
    }

    edge_pairs.resize(p_cnt + 1);
    for (int i = 1; i <= m; ++i) {
        edge_pairs[pair_id[i]].push_back({edges[i].u, edges[i].v});
    }

    RollbackDSU dsu(n);

    // Initial check just in case we don't need to close any doors at all
    // (Though the problem statement guarantees room 1 and 3 are connected initially, 
    // doing this check is harmless and safe).
    int initial_state = dsu.get_state();
    for (int i = 1; i <= p_cnt; ++i) {
        for (auto& edge : edge_pairs[i]) {
            dsu.unite(edge.first, edge.second);
        }
    }
    if (dsu.find(1) == dsu.find(2) && dsu.find(1) != dsu.find(3)) {
        cout << "Ja\n";
        return 0;
    }
    dsu.rollback(initial_state);

    // Kick off the Divide and Conquer
    // It will test disabling pair 1, then pair 2, ..., up to pair p_cnt.
    if (p_cnt > 0) {
        solve(1, p_cnt, dsu);
    }

    // If the recursion completely finishes without calling `exit(0)`, it's impossible.
    cout << "Nej\n";
    return 0;
}