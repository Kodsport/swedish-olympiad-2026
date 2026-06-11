#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int l;
    int g;
    int id;
    
    // Sort primarily by descending density (value / weight). 
    // On ties, prefer lighter items.
    bool operator<(const Item& other) const {
        long long left = (long long)l * other.g;
        long long right = (long long)other.l * g;
        if (left != right) return left > right;
        return g < other.g;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, T;
    if (!(cin >> N >> T)) return 0;

    vector<Item> items;
    items.reserve(N);
    for (int i = 0; i < N; ++i) {
        int l, g;
        cin >> l >> g;
        // Filter out objectively useless elements right away
        if (g <= T) {
            items.push_back({l, g, i});
        }
    }

    N = items.size();
    if (N == 0) {
        cout << 0 << "\n";
        return 0;
    }

    sort(items.begin(), items.end());

    long long current_weight = 0;
    int B = N; // Boundary index
    
    // Find the breaking point of the pure greedy approach
    for (int i = 0; i < N; ++i) {
        if (current_weight + items[i].g > T) {
            B = i;
            break;
        }
        current_weight += items[i].g;
    }

    // If pure greedy takes everything, it is 100% optimal
    if (B == N) {
        long long total_v = 0;
        for (int i = 0; i < N; ++i) total_v += items[i].l;
        cout << total_v << "\n";
        return 0;
    }

    // Window configuration (Massive scale for typical heuristic bounds)
    int K = 10000;
    int start = max(0, B - K / 2);
    int end = min(N - 1, start + K - 1);
    start = max(0, end - K + 1);

    long long W_base = 0;
    long long V_base = 0;
    
    // Forcefully include everything prior to the window
    for (int i = 0; i < start; ++i) {
        W_base += items[i].g;
        V_base += items[i].l;
    }

    int T_rem = T - W_base;
    const int INF = 1e9 + 7;
    int max_possible_v = (end - start + 1) * 5;
    
    // dp[v] = Minimum weight taken exactly from the window bounds to achieve value v.
    vector<int> dp(max_possible_v + 6, INF);
    dp[0] = 0;
    
    int cur_max_v = 0;
    int limit = T_rem;

    // Run heavily pruned DP across the window of items
    for (int i = start; i <= end; ++i) {
        int l = items[i].l;
        int g = items[i].g;
        
        // Looping backwards ensures valid 0/1 knapsack topology
        for (int v = cur_max_v; v >= 0; --v) {
            int w = dp[v] + g;
            if (w <= limit && w < dp[v + l]) {
                dp[v + l] = w;
            }
        }
        cur_max_v += l;
    }

    int best_v = 0;
    for (int v = 0; v <= max_possible_v; ++v) {
        if (dp[v] <= limit) {
            best_v = max(best_v, v);
        }
    }

    cout << V_base + best_v << "\n";
    return 0;
}
