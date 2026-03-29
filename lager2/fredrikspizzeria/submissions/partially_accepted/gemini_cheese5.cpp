#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>

using namespace std;

const int MAXN = 200005;
vector<int> adj[MAXN];
int U[MAXN], V[MAXN], linked_edge[MAXN];
int vis[MAXN], depth[MAXN], parent_edge[MAXN];
int cycle_id[MAXN]; 
vector<vector<int>> cycles;

void dfs_cycles(int u, int p_edge, int d) {
    vis[u] = 1;
    depth[u] = d;
    parent_edge[u] = p_edge;
    for (int e : adj[u]) {
        if (e == p_edge) continue;
        int v = U[e] ^ V[e] ^ u;
        if (vis[v]) {
            if (depth[v] < depth[u]) { // Back-edge defines a cycle
                vector<int> cyc;
                cyc.push_back(e);
                int curr = u;
                while (curr != v) {
                    int pe = parent_edge[curr];
                    cyc.push_back(pe);
                    curr = U[pe] ^ V[pe] ^ curr;
                }
                cycles.push_back(cyc);
                for (int ce : cyc) cycle_id[ce] = cycles.size() - 1;
            }
        } else {
            dfs_cycles(v, e, d + 1);
        }
    }
}

vector<int> path_1_2;
bool vis_path[MAXN];
bool dfs_path(int u, int target) {
    vis_path[u] = true;
    if (u == target) return true;
    for (int e : adj[u]) {
        int v = U[e] ^ V[e] ^ u;
        if (vis_path[v]) continue;
        if (dfs_path(v, target)) {
            path_1_2.push_back(e);
            return true;
        }
    }
    return false;
}

struct CycleChoice {
    vector<int> side1, side2;
};

vector<int> mandatory_edges;
vector<CycleChoice> choices;
int state[MAXN];
auto start_time = chrono::high_resolution_clock::now();

bool check_win() {
    vector<bool> visited(MAXN, false);
    queue<int> q;
    q.push(1);
    visited[1] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == 3) return false; // Rat reached
        for (int e : adj[u]) {
            if (state[e]) {
                int v = U[e] ^ V[e] ^ u;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
    return true; // Rat is isolated
}

bool solve_cheese(int idx) {
    auto now = chrono::high_resolution_clock::now();
    if (chrono::duration_cast<chrono::milliseconds>(now - start_time).count() > 1800) {
        return false; // Abort before TLE
    }
    
    if (idx == choices.size()) {
        return check_win();
    }
    
    int first = rand() % 2;
    for (int step = 0; step < 2; step++) {
        int choice = first ^ step;
        const auto& side = (choice == 0) ? choices[idx].side1 : choices[idx].side2;
        
        vector<int> changed;
        for (int e : side) {
            if (!state[e]) { state[e] = 1; changed.push_back(e); }
            int le = linked_edge[e];
            if (!state[le]) { state[le] = 1; changed.push_back(le); }
        }
        
        if (solve_cheese(idx + 1)) return true;
        
        // Fast O(1) rollback
        for (int e : changed) state[e] = 0;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    srand(1337);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    for (int i = 0; i < m; i++) cycle_id[i] = -1;
    
    for (int i = 0; i < m; i++) {
        int u, v, l;
        cin >> u >> v >> l;
        u--; v--; l--;
        U[i] = u; V[i] = v; linked_edge[i] = l;
        adj[u].push_back(i);
        adj[v].push_back(i);
    }
    
    dfs_cycles(1, -1, 0);
    dfs_path(1, 2);
    
    vector<bool> in_path(m, false);
    for (int e : path_1_2) in_path[e] = true;
    
    for (int i = 0; i < path_1_2.size(); i++) {
        int e = path_1_2[i];
        if (cycle_id[e] == -1) {
            mandatory_edges.push_back(e);
        } else {
            int cid = cycle_id[e];
            CycleChoice cc;
            for (int ce : cycles[cid]) {
                if (in_path[ce]) cc.side1.push_back(ce);
                else cc.side2.push_back(ce);
            }
            choices.push_back(cc);
            while (i + 1 < path_1_2.size() && cycle_id[path_1_2[i+1]] == cid) i++;
        }
    }
    
    for (int e : mandatory_edges) {
        state[e] = 1;
        state[linked_edge[e]] = 1;
    }
    
    random_shuffle(choices.begin(), choices.end());
    
    if (solve_cheese(0)) cout << "Ja\n";
    else cout << "Nej\n";

    return 0;
}