#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int id;
    int l;
    long long g;
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
        if (g <= T) { // Discard items that can't fit even if they are the only element
            items.push_back({i, l, g});
        }
    }

    // Sort by efficiency (l / g) descending. 
    // If tie, sort by g ascending to favor smaller, more flexible items.
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        long long left = (long long)a.l * b.g;
        long long right = (long long)b.l * a.g;
        if (left != right) {
            return left > right;
        }
        if (a.g != b.g) {
            return a.g < b.g;
        }
        return a.id < b.id;
    });

    int n = items.size();
    if (n == 0) {
        cout << 0 << "\n";
        return 0;
    }

    long long current_weight = 0;
    int idx = 0;
    
    // Find the greedy boundary limit
    for (; idx < n; ++idx) {
        if (current_weight + items[idx].g > T) {
            break;
        }
        current_weight += items[idx].g;
    }

    // If all items can be packed perfectly, return total exact sum.
    if (idx == n) {
        long long total_val = 0;
        for (int i = 0; i < n; ++i) {
            total_val += items[i].l;
        }
        cout << total_val << "\n";
        return 0;
    }

    // Calculate a safe window size bounded by max operations to avoid TLE
    long long K = max(600LL, 300000000LL / max(1LL, T));
    K = min(K, (long long)n);

    int L = max(0, idx - (int)K / 2);
    int R = min(n - 1, idx + (int)K / 2);

    // Adjust boundaries to enforce window size == K (if there are enough elements)
    while (R - L + 1 < K) {
        if (L > 0) L--;
        else if (R < n - 1) R++;
        else break;
    }

    long long fixed_weight = 0;
    long long fixed_val = 0;

    for (int i = 0; i < L; ++i) {
        fixed_weight += items[i].g;
        fixed_val += items[i].l;
    }

    // Remaining capacity purely intended for our sliding bounds Window Pool
    long long T_DP = T - fixed_weight;

    vector<int> dp(T_DP + 1, 0);
    int* d = dp.data();

    // 0/1 Knapsack routine on the dynamically evaluated items pool
    for (int i = L; i <= R; ++i) {
        int w = items[i].g;
        int v = items[i].l;
        for (int j = T_DP; j >= w; --j) {
            int cand = d[j - w] + v;
            if (cand > d[j]) {
                d[j] = cand;
            }
        }
    }

    long long ans = fixed_val + dp[T_DP];

    cout << ans << "\n";
    return 0;
}
