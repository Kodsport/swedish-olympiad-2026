#pragma GCC optimize("O3")
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Item {
    long long l, g;
    int id;
    bool operator<(const Item& other) const {
        // Sort primarily by value-to-cost ratio descending
        if (l * other.g != other.l * g) {
            return l * other.g > other.l * g;
        }
        // Tie-breaker: smaller costs first to pack capacities tighter
        return g < other.g; 
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;
    
    vector<Item> items(N);
    for (int i = 0; i < N; ++i) {
        cin >> items[i].l >> items[i].g;
        items[i].id = i;
    }
    
    sort(items.begin(), items.end());
    
    // Find fractional knapsack cutoff M
    long long current_g = 0;
    int M = N;
    for(int i = 0; i < N; ++i) {
        if (current_g + items[i].g > T) {
            M = i;
            break;
        }
        current_g += items[i].g;
    }
    
    // If all items fit into the capacity, trivially take them all
    if (M == N) {
        long long total_l = 0;
        for(int i = 0; i < N; ++i) total_l += items[i].l;
        cout << total_l << "\n";
        return 0;
    }
    
    // Core Window bounds
    int W = 6000;
    int left = max(0, M - W);
    int right = min(N - 1, M + W);
    
    // Items solidly inside the greedy prefix (before the core window)
    long long base_g = 0;
    long long base_l = 0;
    for (int i = 0; i < left; ++i) {
        base_g += items[i].g;
        base_l += items[i].l;
    }
    
    long long rem_T = T - base_g;
    
    int max_core_val = 0;
    for (int i = left; i <= right; ++i) {
        max_core_val += items[i].l;
    }
    
    // 1D DP exactly resolving the subset choices mapped within the core 
    vector<long long> dp(max_core_val + 1, INF);
    dp[0] = 0;
    
    int current_max_v = 0;
    for (int i = left; i <= right; ++i) {
        long long w = items[i].g;
        int v = items[i].l;
        if (v == 0) continue; 
        
        long long* dp_ptr = dp.data();
        for (int j = current_max_v; j >= 0; --j) {
            long long val = dp_ptr[j];
            if (val != INF) {
                long long new_val = val + w;
                if (new_val < dp_ptr[j + v]) {
                    dp_ptr[j + v] = new_val;
                }
            }
        }
        current_max_v += v;
    }
    
    long long best_core_val = 0;
    for (int j = 0; j <= max_core_val; ++j) {
        if (dp[j] <= rem_T) {
            best_core_val = max(best_core_val, (long long)j);
        }
    }
    
    // Our derived heuristic answer
    cout << base_l + best_core_val << "\n";
    
    return 0;
}
