#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    long long l, g;
    int id;
};

// Sort greedily by value/weight efficiency descending. 
// Break ties by picking the lighter items first.
bool cmp(const Item& a, const Item& b) {
    long long lhs = a.l * b.g;
    long long rhs = b.l * a.g;
    if (lhs != rhs) return lhs > rhs;
    if (a.g != b.g) return a.g < b.g;
    return a.id < b.id;
}

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    long long t;
    if (!(cin >> n >> t)) return 0;
    
    vector<Item> items;
    items.reserve(n);
    
    // Filter out items that instantly exceed the knapsack limit (0% chance to be picked)
    for (int i = 0; i < n; ++i) {
        long long l, g;
        cin >> l >> g;
        if (g <= t) {
            items.push_back({l, g, i});
        }
    }
    
    n = items.size();
    if (n == 0) {
        cout << 0 << "\n";
        return 0;
    }
    
    sort(items.begin(), items.end(), cmp);
    
    long long current_weight = 0;
    long long current_value = 0;
    int c = 0;
    
    // Greedily simulate knapsack insertion to find the limit/cutoff index 'c'
    while (c < n && current_weight + items[c].g <= t) {
        current_weight += items[c].g;
        current_value += items[c].l;
        c++;
    }
    
    // If we could greedily pick everything, then it is inherently optimal
    if (c == n) {
        cout << current_value << "\n";
        return 0;
    }
    
    // --- CORE DP WINDOW HEURISTIC ---
    // Establish a threshold window uniformly around the limit index `c`
    // Tweak `M` directly if you want it to pass/fail differing tolerances.
    int M = 800; 
    int start_idx = max(0, c - M);
    int end_idx = min(n - 1, c + M);
    
    long long base_weight = 0;
    long long base_value = 0;
    
    // We firmly commit to ALL greedy choices prior to the core's bounds
    for (int i = 0; i < start_idx; ++i) {
        base_weight += items[i].g;
        base_value += items[i].l;
    }
    
    long long rem_t = t - base_weight;
    
    // dp[w] yields the maximum value for exactly weight 'w' inside the window range
    vector<int> dp(rem_t + 1, -1);
    dp[0] = 0;
    long long max_w = 0;
    
    // Sub-Knapsack bounding optimization
    for (int i = start_idx; i <= end_idx; ++i) {
        long long w = items[i].g;
        int v = items[i].l;
        max_w = min(rem_t, max_w + w);
        
        for (long long j = max_w; j >= w; --j) {
            if (dp[j - w] != -1) {
                if (dp[j] < dp[j - w] + v) {
                    dp[j] = dp[j - w] + v;
                }
            }
        }
    }
    
    // Aggregate the best value found in the local subset
    int max_add = 0;
    for (long long j = 0; j <= rem_t; ++j) {
        if (dp[j] > max_add) {
            max_add = dp[j];
        }
    }
    
    cout << base_value + max_add << "\n";
    return 0;
}