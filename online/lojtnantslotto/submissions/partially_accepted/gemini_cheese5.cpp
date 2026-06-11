#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct Item {
    int l;
    long long g;
    int id;
};

// Sort by value/weight ratio descending. Break ties with smaller weight ascending.
bool compareItems(const Item& a, const Item& b) {
    long long left = (long long)a.l * b.g;
    long long right = (long long)b.l * a.g;
    if (left != right) return left > right;
    return a.g < b.g;
}

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    long long T;
    if (!(cin >> n >> T)) return 0;
    
    vector<Item> items;
    for (int i = 0; i < n; ++i) {
        int l;
        long long g;
        cin >> l >> g;
        if (g <= T) { // Items with weight > T are unconditionally useless
            items.push_back({l, g, i});
        }
    }
    
    n = items.size();
    if (n == 0) {
        cout << 0 << "\n";
        return 0;
    }
    
    sort(items.begin(), items.end(), compareItems);
    
    long long current_weight = 0;
    int idx = n;
    
    // Greedily find the knapsack's breaking point
    for (int i = 0; i < n; ++i) {
        if (current_weight + items[i].g > T) {
            idx = i;
            break;
        }
        current_weight += items[i].g;
    }
    
    // If all valid items fit in the knapsack natively
    if (idx == n) {
        long long total_val = 0;
        for (int i = 0; i < n; ++i) total_val += items[i].l;
        cout << total_val << "\n";
        return 0;
    }
    
    // Core parameters (Determines the vulnerability limit of the heuristic)
    int CORE_HALF = 3000;
    int L = max(0, idx - CORE_HALF);
    int R = min(n - 1, idx + CORE_HALF);
    
    // Force optimal exact DP to safely clear Group 3 (N <= 5000)
    if (n <= 5000) {
        L = 0;
        R = n - 1;
    }
    
    long long w_base = 0;
    long long v_base = 0;
    
    // Permanently take items to the left of the core
    for (int i = 0; i < L; ++i) {
        w_base += items[i].g;
        v_base += items[i].l;
    }
    
    long long rem_T = T - w_base;
    int max_val = 0;
    for (int i = L; i <= R; ++i) {
        max_val += items[i].l;
    }
    
    // Cap `max_val` because we logically can't accrue more value than rem_T * max_single_value(5)
    long long max_val_by_rem = (long long)rem_T * 5;
    if (max_val_by_rem < max_val) {
        max_val = (int)max_val_by_rem;
    }
    
    const long long INF = 1e18;
    vector<long long> dp(max_val + 1, INF);
    dp[0] = 0;
    
    int current_max_val = 0;
    
    // Run exact DP bounds exclusively on the Core subset
    for (int i = L; i <= R; ++i) {
        int v = items[i].l;
        long long w = items[i].g;
        
        current_max_val = min(max_val, current_max_val + v);
        for (int j = current_max_val; j >= v; --j) {
            if (dp[j - v] != INF && dp[j - v] + w < dp[j]) {
                dp[j] = dp[j - v] + w;
            }
        }
    }
    
    long long best_extra = 0;
    for (int j = 0; j <= max_val; ++j) {
        if (dp[j] <= rem_T) {
            best_extra = j;
        }
    }
    
    cout << v_base + best_extra << "\n";
    return 0;
}
