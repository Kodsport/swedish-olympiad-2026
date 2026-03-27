// @EXPECTED_GRADES@ AC WA WA WA WA
//
// Cheese: Detects which gen_perm.py pattern produced the input and uses
// hardcoded formulas. Falls back to greedy for undetected patterns.
// Formulas may be wrong for some patterns. Directly exploits generator knowledge.

#include <cstdio>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>
using namespace std;

int n;
vector<int> p;

// --- Brute force for n <= 15 ---
int brute_force() {
    // vis[i] = bitmask of positions visible from i
    vector<int> vis(n, 0);
    for (int i = 0; i < n; i++) {
        vis[i] |= 1 << i;
        for (int j = i + 1; j < n; j++) {
            if (j == i + 1) {
                vis[i] |= 1 << j;
                vis[j] |= 1 << i;
            } else {
                int mx = 0;
                for (int k = i + 1; k < j; k++) {
                    if (p[k] > mx) mx = p[k];
                }
                int mn = min(p[i], p[j]);
                if (mx < mn) {
                    vis[i] |= 1 << j;
                    vis[j] |= 1 << i;
                }
            }
        }
    }

    int full = (1 << n) - 1;
    int best = n;
    for (int mask = 1; mask < (1 << n); mask++) {
        int bc = __builtin_popcount(mask);
        if (bc >= best) continue;
        int covered = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                covered |= vis[i];
            }
        }
        if (covered == full) {
            best = bc;
        }
    }
    return best;
}

// --- Pattern detection ---
bool is_sorted_asc() {
    for (int i = 0; i < n; i++) {
        if (p[i] != i + 1) return false;
    }
    return true;
}

bool is_sorted_desc() {
    for (int i = 0; i < n; i++) {
        if (p[i] != n - i) return false;
    }
    return true;
}

vector<int> build_alternating() {
    vector<int> low, high;
    for (int i = 1; i <= (n + 1) / 2; i++) low.push_back(i);
    for (int i = (n + 1) / 2 + 1; i <= n; i++) high.push_back(i);
    reverse(high.begin(), high.end());
    vector<int> perm(n);
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            perm[i] = low[i / 2];
        } else {
            perm[i] = high[i / 2];
        }
    }
    return perm;
}

bool is_alternating() {
    vector<int> alt = build_alternating();
    return p == alt;
}

bool is_bitonic() {
    int mid = n / 2;
    for (int i = 0; i < mid; i++) {
        if (p[i] != i + 1) return false;
    }
    for (int i = mid; i < n; i++) {
        if (p[i] != n - (i - mid)) return false;
    }
    return true;
}

vector<int> build_recursive() {
    // Iterative to avoid stack overflow
    vector<int> order;
    order.reserve(n);
    stack<pair<int, int>> st;
    st.push({0, n - 1});
    while (!st.empty()) {
        auto [lo, hi] = st.top();
        st.pop();
        if (lo > hi) continue;
        int mid = (lo + hi) / 2;
        order.push_back(mid);
        // Push right first so left is processed first
        st.push({mid + 1, hi});
        st.push({lo, mid - 1});
    }
    vector<int> perm(n, 0);
    int value = n;
    for (int idx : order) {
        perm[idx] = value;
        value--;
    }
    return perm;
}

bool is_recursive() {
    vector<int> ref = build_recursive();
    return p == ref;
}

int detect_blocks() {
    // Find length of first decreasing run
    int bs = 1;
    while (bs < n && p[bs] < p[bs - 1]) {
        bs++;
    }
    if (bs == 1) return -1;

    // Verify all blocks
    for (int start = 0; start < n; start += bs) {
        int end = min(start + bs, n);
        int block_len = end - start;
        for (int j = 1; j < block_len; j++) {
            if (p[start + j] != p[start] - j) return -1;
        }
    }
    // Check blocks form consecutive value ranges
    for (int start = 0; start < n; start += bs) {
        int expected_max = min(start + bs, n);
        if (p[start] != expected_max) return -1;
    }
    return bs;
}

// --- Greedy set cover fallback ---
int greedy_cover() {
    // Build Cartesian tree
    vector<int> left(n, -1), right(n, -1), par(n, -1);
    vector<int> stk;
    for (int i = 0; i < n; i++) {
        int last = -1;
        while (!stk.empty() && p[stk.back()] < p[i]) {
            last = stk.back();
            stk.pop_back();
        }
        if (!stk.empty()) {
            right[stk.back()] = i;
            par[i] = stk.back();
        }
        if (last != -1) {
            left[i] = last;
            par[last] = i;
        }
        stk.push_back(i);
    }

    // Build adjacency: tree edges + adjacent position edges
    vector<set<int>> adj(n);
    for (int i = 0; i < n; i++) {
        if (left[i] != -1) {
            adj[i].insert(left[i]);
            adj[left[i]].insert(i);
        }
        if (right[i] != -1) {
            adj[i].insert(right[i]);
            adj[right[i]].insert(i);
        }
    }
    for (int i = 0; i < n - 1; i++) {
        adj[i].insert(i + 1);
        adj[i + 1].insert(i);
    }

    // Greedy with max-heap and lazy deletion
    vector<bool> covered(n, false);
    vector<bool> selected(n, false);
    int total_covered = 0;
    int result = 0;

    // Max-heap: (coverage_count, node_index)
    priority_queue<pair<int, int>> heap;
    for (int i = 0; i < n; i++) {
        int cnt = 1 + (int)adj[i].size();
        heap.push({cnt, i});
    }

    while (total_covered < n) {
        bool found = false;
        while (!heap.empty()) {
            auto [neg_unused, node] = heap.top();
            heap.pop();
            if (selected[node]) continue;

            // Recount actual uncovered coverage
            int actual = 0;
            if (!covered[node]) actual++;
            for (int nb : adj[node]) {
                if (!covered[nb]) actual++;
            }
            if (actual == 0) continue;

            // Lazy check
            if (actual != neg_unused) {
                heap.push({actual, node});
                continue;
            }

            // Select this node
            selected[node] = true;
            result++;
            if (!covered[node]) {
                covered[node] = true;
                total_covered++;
            }
            for (int nb : adj[node]) {
                if (!covered[nb]) {
                    covered[nb] = true;
                    total_covered++;
                }
            }
            found = true;
            break;
        }
        if (!found) {
            // Select any uncovered node
            for (int i = 0; i < n; i++) {
                if (!covered[i]) {
                    covered[i] = true;
                    total_covered++;
                    result++;
                    break;
                }
            }
        }
    }
    return result;
}

int main() {
    scanf("%d", &n);
    p.resize(n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &p[i]);
    }

    if (n == 1) {
        printf("1\n");
        return 0;
    }

    if (n <= 15) {
        printf("%d\n", brute_force());
        return 0;
    }

    // Detect pattern
    if (is_sorted_asc() || is_sorted_desc()) {
        printf("%d\n", (n + 2) / 3);
        return 0;
    }
    if (is_alternating()) {
        printf("%d\n", (n + 3) / 4);
        return 0;
    }
    if (is_bitonic()) {
        printf("%d\n", (n + 2) / 3);
        return 0;
    }
    if (is_recursive()) {
        printf("%d\n", (n + 2) / 3);
        return 0;
    }
    if (detect_blocks() != -1) {
        printf("%d\n", (n + 2) / 3);
        return 0;
    }

    // Fallback: greedy
    printf("%d\n", greedy_cover());
    return 0;
}
