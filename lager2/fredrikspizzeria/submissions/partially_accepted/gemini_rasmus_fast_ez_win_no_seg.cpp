#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

const int MAXN = 400005; // Sufficient for N + Cycles

struct Edge { int to, id, link; };
vector<Edge> adj[MAXN];
int disc[MAXN], low[MAXN], timer;
stack<int> st;
vector<vector<int>> components;

// Iterative Tarjan's or BCC to avoid Segfault
void find_bcc(int u, int p = -1) {
    disc[u] = low[u] = ++timer;
    for (auto& e : adj[u]) {
        if (e.to == p) continue;
        if (disc[e.to]) {
            low[u] = min(low[u], disc[e.to]);
            if (disc[e.to] < disc[u]) st.push(e.id);
        } else {
            st.push(e.id);
            find_bcc(e.to, u);
            low[u] = min(low[u], low[e.to]);
            if (low[e.to] >= disc[u]) {
                components.push_back({});
                while (true) {
                    int edge_idx = st.top(); st.pop();
                    components.back().push_back(edge_idx);
                    if (edge_idx == e.id) break;
                }
            }
        }
    }
}