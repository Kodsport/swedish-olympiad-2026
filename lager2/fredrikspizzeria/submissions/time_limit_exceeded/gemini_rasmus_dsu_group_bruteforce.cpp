#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

// Disjoint Set Union to find equivalence classes of corridors
struct DSU {
    vector<int> parent;
    DSU(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
        }
    }
};

struct Edge {
    int u, v, id;
};

// Recursive backtracking to check all 2^K group states
bool backtrack(int idx, const vector<int>& groups, vector<bool>& state, 
               const vector<Edge>& edges, int n, DSU& dsu) {
    
    // Base Case: All group states are assigned
    if (idx == groups.size()) {
        vector<vector<int>> adj(n + 1);
        
        // Build the graph using only open corridors
        for (size_t i = 1; i < edges.size(); ++i) {
            int root = dsu.find(i);
            if (state[root]) {
                adj[edges[i].u].push_back(edges[i].v);
                adj[edges[i].v].push_back(edges[i].u);
            }
        }

        // BFS to find reachable rooms from Room 1
        vector<bool> visited(n + 1, false);
        vector<int> q;
        q.push_back(1);
        visited[1] = true;
        int head = 0;
        
        while (head < q.size()) {
            int u = q[head++];
            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push_back(v);
                }
            }
        }

        // Valid if Exit (2) is reached and Rat (3) is isolated
        return (visited[2] && !visited[3]);
    }

    int current_group = groups[idx];

    // Branch 1: Try leaving this group of corridors OPEN
    state[current_group] = true;
    if (backtrack(idx + 1, groups, state, edges, n, dsu)) return true;

    // Branch 2: Try leaving this group of corridors CLOSED
    state[current_group] = false;
    if (backtrack(idx + 1, groups, state, edges, n, dsu)) return true;

    return false;
}

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<Edge> edges(m + 1);
    DSU dsu(m);

    for (int i = 1; i <= m; ++i) {
        int u, v, o;
        cin >> u >> v >> o;
        edges[i] = {u, v, i};
        dsu.unite(i, o);
    }

    vector<int> groups;
    for (int i = 1; i <= m; ++i) {
        if (dsu.parent[i] == i) {
            groups.push_back(i);
        }
    }

    vector<bool> state(m + 1, false);

    if (backtrack(0, groups, state, edges, n, dsu)) {
        cout << "Ja\n";
    } else {
        cout << "Nej\n";
    }

    return 0;
}
