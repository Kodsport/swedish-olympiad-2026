// gemini_cheese11.cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Store items securely for global tracking
struct Item {
    long long v, w;
    int id;
    
    // Sort primarily by efficiency v/w descending
    // Break ties by picking the larger value (better density), then smaller weight
    bool operator<(const Item& o) const {
        if (v * o.w != o.v * w) return v * o.w > o.v * w;
        if (v != o.v) return v > o.v;
        return w < o.w;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;

    vector<Item> all_items;
    vector<long long> groups[6];

    for (int i = 0; i < N; ++i) {
        long long l, g;
        cin >> l >> g;
        all_items.push_back({l, g, i});
        groups[l].push_back(g);
    }

    // Sort weights within each value group ascendingly
    for (int v = 1; v <= 5; ++v) {
        sort(groups[v].begin(), groups[v].end());
    }

    sort(all_items.begin(), all_items.end());

    // Calculate Prefix sums for rapid continuous lookups
    vector<long long> pref[6];
    for (int v = 1; v <= 5; ++v) {
        pref[v].push_back(0);
        long long sum = 0;
        for (long long w : groups[v]) {
            sum += w;
            pref[v].push_back(sum);
        }
    }

    // Pass 1: Baseline Greedy Execution
    vector<int> C(6, 0);
    long long current_weight = 0;
    for (const auto& item : all_items) {
        if (current_weight + item.w <= T) {
            current_weight += item.w;
            C[item.v]++;
        }
    }

    // Pass 2: Greedy-Anchored Windowed Dynamic Programming
    // Limit deviation from the greedy choice to create a high-accuracy heuristic
    int W_WINDOW = 300; 
    int max_val = 5 * N;
    
    vector<long long> dp(max_val + 1, 1e18);
    vector<long long> new_dp(max_val + 1, 1e18);
    dp[0] = 0;

    int current_min_val = 0;
    int current_max_val = 0;

    for (int v = 1; v <= 5; ++v) {
        if (pref[v].size() == 1) continue; // No items represented under this group value

        int start_j = max(0, C[v] - W_WINDOW);
        int end_j = min((int)pref[v].size() - 1, C[v] + W_WINDOW);
        
        int next_min = 1e9, next_max = -1;
        
        for (int val = current_min_val; val <= current_max_val; ++val) {
            if (dp[val] >= 1e18) continue;
            
            for (int j = start_j; j <= end_j; ++j) {
                long long w = pref[v][j];
                if (dp[val] + w <= T) {
                    int next_val = val + j * v;
                    if (dp[val] + w < new_dp[next_val]) {
                        new_dp[next_val] = dp[val] + w;
                    }
                    if (next_val < next_min) next_min = next_val;
                    if (next_val > next_max) next_max = next_val;
                }
            }
        }
        
        // Break out safety net
        if (next_min > next_max) break;
        
        // Swap mechanics / Shift state
        for (int val = current_min_val; val <= current_max_val; ++val) {
            dp[val] = 1e18; 
        }
        for (int val = next_min; val <= next_max; ++val) {
            dp[val] = new_dp[val];
            new_dp[val] = 1e18; 
        }
        current_min_val = next_min;
        current_max_val = next_max;
    }

    // Retract Optimal configuration
    int ans = 0;
    for (int val = 0; val <= max_val; ++val) {
        if (dp[val] <= T) {
            ans = max(ans, val);
        }
    }

    cout << ans << "\n";
    return 0;
}
