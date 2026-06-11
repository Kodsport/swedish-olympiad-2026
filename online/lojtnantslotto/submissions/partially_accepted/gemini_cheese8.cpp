#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Represents a castle (item)
struct Item {
    long long l, g;
    int id;
    
    // Sort descending by value-to-weight ratio (l / g)
    bool operator<(const Item& other) const {
        long long left = l * other.g;
        long long right = other.l * g;
        if (left != right) return left > right;
        if (g != other.g) return g < other.g;     // Tie break: smaller weight
        return id < other.id;                     // Deterministic
    }
};

int main() {
    // Optimize standard I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long t;
    if (!(cin >> n >> t)) return 0;

    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        cin >> items[i].l >> items[i].g;
        items[i].id = i;
    }

    sort(items.begin(), items.end());

    // 1. Locate the greedy split boundary `p`
    long long current_weight = 0;
    int p = n;
    for (int i = 0; i < n; i++) {
        if (current_weight + items[i].g > t) {
            p = i;
            break;
        }
        current_weight += items[i].g;
    }

    // Handles the edge case where we can afford literally all castles 
    if (p == n) {
        long long total_val = 0;
        for (int i = 0; i < n; i++) {
            total_val += items[i].l;
        }
        cout << total_val << "\n";
        return 0;
    }

    // 2. Window scope (heuristics boundary limit)
    // Limits bounding scope to 4000 items total. Guarantees AC <= 5000 exactly
    int K = 2000;
    if (n <= 5000) K = n;

    int start_idx = max(0, p - K);
    int end_idx = min(n - 1, p + K);

    // 3. Establish the base constraints of items placed safely *before* the Core Scope
    long long base_val = 0;
    long long base_weight = 0;
    for (int i = 0; i < start_idx; i++) {
        base_val += items[i].l;
        base_weight += items[i].g;
    }

    long long cap = t - base_weight;     // Residual Capacity
    int max_v = 0;
    for (int i = start_idx; i <= end_idx; i++) {
        max_v += items[i].l;
    }

    // 4. Inverted Dynamic Programming localized to Core elements ONLY
    // dp[v] = minimal weight to achieve precisely value `v` mapped on the residual bounds
    vector<long long> dp(max_v + 1, cap + 1);
    dp[0] = 0;
    int current_max_v = 0;

    for (int i = start_idx; i <= end_idx; i++) {
        long long w = items[i].g;
        int v = items[i].l;
        for (int j = current_max_v; j >= 0; j--) {
            long long next_w = dp[j] + w;
            if (next_w <= cap && next_w < dp[j + v]) {
                dp[j + v] = next_w;
            }
        }
        current_max_v += v;
    }

    long long best_extra_val = 0;
    for (int j = 0; j <= max_v; j++) {
        if (dp[j] <= cap) {
            best_extra_val = max(best_extra_val, (long long)j);
        }
    }

    // Accumulate definitive pre-window base findings with optimized heuristic choices
    cout << base_val + best_extra_val << "\n";

    return 0;
}
