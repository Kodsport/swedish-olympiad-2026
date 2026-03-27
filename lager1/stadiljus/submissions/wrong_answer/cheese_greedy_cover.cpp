// @EXPECTED_GRADES@ WA WA WA WA WA
//
// Cheese: Greedy set cover on the visibility graph. Repeatedly lights the
// lighthouse covering the most uncovered positions. Finds a valid cover
// but not necessarily minimum, so the answer is >= optimal.

#include <cstdio>
#include <vector>
#include <queue>
#include <set>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);

    vector<int> p(n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &p[i]);
    }

    if (n == 1) {
        printf("1\n");
        return 0;
    }

    // Build Cartesian tree using monotonic stack (max-heap)
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

    // Build adjacency list: tree edges + adjacent position edges
    // Use sets to deduplicate
    vector<set<int>> adj_set(n);
    for (int i = 0; i < n; i++) {
        if (left[i] != -1) {
            adj_set[i].insert(left[i]);
            adj_set[left[i]].insert(i);
        }
        if (right[i] != -1) {
            adj_set[i].insert(right[i]);
            adj_set[right[i]].insert(i);
        }
        if (i + 1 < n) {
            adj_set[i].insert(i + 1);
            adj_set[i + 1].insert(i);
        }
    }

    // Convert to vectors for fast iteration
    vector<vector<int>> adj(n);
    for (int i = 0; i < n; i++) {
        adj[i].assign(adj_set[i].begin(), adj_set[i].end());
    }
    adj_set.clear();

    // Coverage set for each node: {itself} + neighbors
    // uncov_count[i] = number of uncovered nodes in coverage_set[i]
    vector<bool> covered(n, false);
    vector<int> uncov_count(n);
    for (int i = 0; i < n; i++) {
        uncov_count[i] = 1 + (int)adj[i].size();
    }

    // Max-heap: (uncov_count, index)
    priority_queue<pair<int, int>> heap;
    for (int i = 0; i < n; i++) {
        heap.push({uncov_count[i], i});
    }

    int ans = 0;
    int total_covered = 0;

    while (total_covered < n) {
        // Pop until we find a valid entry
        int x = -1;
        while (!heap.empty()) {
            auto [cnt, idx] = heap.top();
            heap.pop();
            if (cnt == uncov_count[idx] && uncov_count[idx] > 0) {
                x = idx;
                break;
            }
        }
        if (x == -1) break;

        // Light node x
        ans++;

        // Find newly covered nodes in coverage_set[x]
        vector<int> newly_covered;
        if (!covered[x]) {
            newly_covered.push_back(x);
        }
        for (int nb : adj[x]) {
            if (!covered[nb]) {
                newly_covered.push_back(nb);
            }
        }

        // Mark them as covered
        for (int m : newly_covered) {
            covered[m] = true;
        }
        total_covered += (int)newly_covered.size();

        // Update uncov_count for affected nodes
        for (int m : newly_covered) {
            // m is now covered, so for every node that has m in its coverage set,
            // decrement their uncov_count. m's coverage set neighbors are adj[m],
            // plus m itself.
            uncov_count[m]--;
            if (uncov_count[m] >= 0) {
                heap.push({uncov_count[m], m});
            }
            for (int nb : adj[m]) {
                uncov_count[nb]--;
                if (uncov_count[nb] >= 0) {
                    heap.push({uncov_count[nb], nb});
                }
            }
        }
    }

    printf("%d\n", ans);
    return 0;
}
