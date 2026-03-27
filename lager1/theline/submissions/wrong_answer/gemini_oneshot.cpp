#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

// Lazy Segment tree to maintain the maximums of our swept equations
struct LazySegmentTree {
    int n;
    vector<long long> tree, lazy;

    LazySegmentTree(int size) {
        n = size;
        tree.assign(4 * n + 1, 0);
        lazy.assign(4 * n + 1, 0);
    }

    void push(int node) {
        if (lazy[node] != 0) {
            tree[2 * node] += lazy[node];
            lazy[2 * node] += lazy[node];
            tree[2 * node + 1] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
            lazy[node] = 0;
        }
    }

    void update(int node, int start, int end, int l, int r, long long val) {
        if (r < start || end < l) return;
        if (l <= start && end <= r) {
            tree[node] += val;
            lazy[node] += val;
            return;
        }
        push(node);
        int mid = (start + end) / 2;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

    void update(int l, int r, long long val) {
        if (l > r) return;
        update(1, 0, n, l, r, val);
    }

    long long query_max() {
        return tree[1];
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    long long original_total_dist = 0;
    struct Path { int u, v, d, c; };
    vector<Path> paths(K);

    for (int i = 0; i < K; i++) {
        int s, t;
        cin >> s >> t;
        paths[i].u = min(s, t);
        paths[i].v = max(s, t);
        paths[i].d = paths[i].v - paths[i].u;
        paths[i].c = paths[i].v + paths[i].u;
        original_total_dist += paths[i].d;
    }

    // A mapping matrix to log the sweeping events on endpoints
    // events[x] will store vectors of {y_start, y_end, value_to_add}
    vector<vector<tuple<int, int, int>>> events(N + 2);

    for (int i = 0; i < K; i++) {
        if (paths[i].d <= 1) continue; // No saving possible
        
        int U = paths[i].u;
        int V = paths[i].v;
        
        // As x increases, functions change linearly based on bounds. We record the deltas mapping back into Y updates
        // To maintain strict N log N bound, we queue updates onto our 1D SegTree for moving boundaries
        for (int x = max(1, 2 * U - V + 1); x < U; x++) {
            int H = V - 2 * U + x - 1;
            if (H >= 0) events[x + 1].emplace_back(V - H, V + H, 1);
        }
        for (int x = U; x <= V - 2; x++) {
            int H = V - x - 1;
            if (H - 1 >= 0) events[x + 1].emplace_back(V - H + 1, V + H - 1, -1);
        }
    }

    LazySegmentTree seg_tree(N + 1);
    long long max_saved = 0;

    // Evaluate base cases into tree before the sweep
    for(int i = 0; i < K; i++){
        int U = paths[i].u, V = paths[i].v;
        int H = V - 2 * U; 
        if(H > 0) seg_tree.update(V - H, V + H, 1);
    }

    for (int x = 1; x <= N; x++) {
        // Apply differential events for the current X slice 
        for (auto& ev : events[x]) {
            int l = get<0>(ev);
            int r = get<1>(ev);
            int val = get<2>(ev);
            seg_tree.update(l, r, val);
        }
        
        // Find optimal Y for the current X 
        max_saved = max(max_saved, seg_tree.query_max());
    }

    cout << original_total_dist - max_saved << "\n";
    return 0;
}
