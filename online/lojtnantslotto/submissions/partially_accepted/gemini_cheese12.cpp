#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int l;
    long long g;
    
    // Sort primarily by density (l/g) descending, then by weight ascending
    bool operator<(const Item& other) const {
        long long left = (long long)l * other.g;
        long long right = (long long)other.l * g;
        if (left != right) return left > right;
        return g < other.g;
    }
};

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;

    vector<Item> items;
    items.reserve(N);
    
    for (int i = 0; i < N; ++i) {
        int l;
        long long g;
        cin >> l >> g;
        // Optimization: immediately discard items that individually exceed our total troops
        if (g <= T) {
            items.push_back({l, g});
        }
    }

    if (items.empty()) {
        cout << 0 << "\n";
        return 0;
    }

    // Sort to prepare for greedy sequence / core identification
    sort(items.begin(), items.end());

    int n = items.size();
    long long cur_g = 0;
    long long cur_l = 0;
    int split = -1;

    // Find the greedy split point
    for (int i = 0; i < n; ++i) {
        if (cur_g + items[i].g > T) {
            split = i;
            break;
        }
        cur_g += items[i].g;
        cur_l += items[i].l;
    }

    // Edge case: if we can plunder all viable castles without running out of troops
    if (split == -1) {
        cout << cur_l << "\n";
        return 0;
    }

    // M defines the half-size of the core. M=3000 yields up to 6000 items in DP.
    int M = 3000;
    int L = max(0, split - M);
    int R = min(n - 1, split + M);

    // Items strictly before `L` are definitively locked into our solution
    long long base_w = 0;
    long long base_v = 0;
    for (int i = 0; i < L; ++i) {
        base_w += items[i].g;
        base_v += items[i].l;
    }

    long long C = T - base_w; // Capacity leftover for the core

    int max_core_v = 0;
    for (int i = L; i <= R; ++i) {
        max_core_v += items[i].l;
    }

    // dp[v] := minimum weight required to achieve exact plunder value 'v' from core items
    vector<long long> dp(max_core_v + 1, 2e18);
    dp[0] = 0;
    
    int current_v = 0; // Tightly bounds the inner-loop avoiding useless iterations
    for (int i = L; i <= R; ++i) {
        int l = items[i].l;
        long long g = items[i].g;
        long long limit = C - g;
        
        // Compute standard 0-1 knapsack backward array mappings 
        for (int v = current_v; v >= 0; --v) {
            if (dp[v] <= limit) {
                long long cand = dp[v] + g;
                if (cand < dp[v + l]) {
                    dp[v + l] = cand;
                }
            }
        }
        current_v += l;
    }

    long long best_v = 0;
    for (int v = 0; v <= max_core_v; ++v) {
        if (dp[v] <= C) {
            best_v = max(best_v, (long long)v);
        }
    }

    // Final result comprises locked initial items + optimally solved core window
    cout << base_v + best_v << "\n";

    return 0;
}
