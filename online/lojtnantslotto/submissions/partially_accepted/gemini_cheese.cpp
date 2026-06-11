#pragma GCC optimize("O3,unroll-loops")
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int l, g;
    
    // Sort primarily by value-to-weight ratio (descending)
    // Sort secondarily by weight (ascending) to prefer smaller chunks
    bool operator<(const Item& other) const {
        long long left = (long long)l * other.g;
        long long right = (long long)other.l * g;
        if (left != right) {
            return left > right;
        }
        return g < other.g;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, T;
    if (!(cin >> N >> T)) return 0;

    vector<Item> items(N);
    for (int i = 0; i < N; ++i) {
        cin >> items[i].l >> items[i].g;
    }

    sort(items.begin(), items.end());

    // Phase 1: Greedily pack items to establish the split element 'S'
    long long current_weight = 0;
    int S = N;
    for (int i = 0; i < N; ++i) {
        if (current_weight + items[i].g > T) {
            S = i;
            break;
        }
        current_weight += items[i].g;
    }

    // Edge case: All items can be looted
    if (S == N) {
        long long total_value = 0;
        for (int i = 0; i < N; ++i) {
            total_value += items[i].l;
        }
        cout << total_value << "\n";
        return 0;
    }

    // Phase 2: Define the Core Window limits
    int C = 400; // Core radius. Increase this to slow down the approach and increase accuracy.
    int L = max(0, S - C);
    int R = min(N - 1, S + C);

    // Pack everything located strictly before the core's bounds
    long long W_base = 0;
    long long V_base = 0;
    for (int i = 0; i < L; ++i) {
        W_base += items[i].g;
        V_base += items[i].l;
    }

    // Phase 3: Run exact 0/1 Knapsack exclusively on the Core
    int T_core = T - W_base;
    vector<int> dp(T_core + 1, 0);

    int current_sum = 0;
    for (int i = L; i <= R; ++i) {
        int w = items[i].g;
        int v = items[i].l;
        current_sum = min(T_core, current_sum + w);
        
        for (int j = current_sum; j >= w; --j) {
            int nv = dp[j - w] + v;
            if (nv > dp[j]) {
                dp[j] = nv;
            }
        }
    }

    int max_dp = 0;
    for (int j = 0; j <= T_core; ++j) {
        if (dp[j] > max_dp) {
            max_dp = dp[j];
        }
    }

    cout << V_base + max_dp << "\n";
    return 0;
}
