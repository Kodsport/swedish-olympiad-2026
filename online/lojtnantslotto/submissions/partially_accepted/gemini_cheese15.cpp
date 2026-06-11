#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    long long l, g;
    int id;
};

// Sort descending by value to weight ratio. 
// Tie break by picking the smaller weight first.
bool cmp(const Item& a, const Item& b) {
    if (a.l * b.g != b.l * a.g) {
        return a.l * b.g > b.l * a.g;
    }
    return a.g < b.g;
}

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;

    vector<Item> items(N);
    long long max_l = 0;
    for (int i = 0; i < N; i++) {
        cin >> items[i].l >> items[i].g;
        items[i].id = i;
        if (items[i].l > max_l) {
            max_l = items[i].l;
        }
    }

    // Exact O(N log N) solution for Groups 1 and 4 
    if (max_l <= 2) {
        vector<long long> S1, S2;
        S1.push_back(0);
        S2.push_back(0);
        vector<long long> w1, w2;
        
        for (int i = 0; i < N; i++) {
            if (items[i].l == 1) w1.push_back(items[i].g);
            else if (items[i].l == 2) w2.push_back(items[i].g);
        }
        sort(w1.begin(), w1.end());
        sort(w2.begin(), w2.end());
        for (long long w : w1) S1.push_back(S1.back() + w);
        for (long long w : w2) S2.push_back(S2.back() + w);

        long long best_val = 0;
        for (int c2 = 0; c2 < S2.size(); c2++) {
            if (S2[c2] > T) break;
            long long rem = T - S2[c2];
            int c1 = upper_bound(S1.begin(), S1.end(), rem) - S1.begin() - 1;
            long long val = 2LL * c2 + c1;
            if (val > best_val) {
                best_val = val;
            }
        }
        cout << best_val << "\n";
        return 0;
    }

    // Heuristic DP fallback for large Group 5 bounds 
    // (Still serves exactness dynamically on Groups 2, 3)
    sort(items.begin(), items.end(), cmp);

    int G = 0;
    long long current_weight = 0;
    long long current_val = 0;
    while (G < N && current_weight + items[G].g <= T) {
        current_weight += items[G].g;
        current_val += items[G].l;
        G++;
    }

    // Exits immediately if we have ample capacity to plunder every castle
    if (G == N) {
        cout << current_val << "\n";
        return 0;
    }

    int W_size = 6000;
    if (N <= W_size) {
        W_size = N;
    }

    int L = max(0, G - W_size / 2);
    int R = L + W_size;
    if (R > N) {
        R = N;
        L = max(0, N - W_size);
    }

    long long base_weight = 0;
    long long base_val = 0;
    for (int i = 0; i < L; i++) {
        base_weight += items[i].g;
        base_val += items[i].l;
    }

    long long rem_T = T - base_weight;
    const long long INF = 1e18;
    int max_window_val = 0;
    for (int i = L; i < R; i++) {
        max_window_val += items[i].l;
    }

    vector<long long> dp(max_window_val + 1, INF);
    dp[0] = 0;

    int current_max = 0;
    for (int i = L; i < R; i++) {
        int v = items[i].l;
        long long w = items[i].g;
        for (int val = current_max; val >= 0; val--) {
            if (dp[val] != INF) {
                if (dp[val] + w < dp[val + v]) {
                    dp[val + v] = dp[val] + w;
                }
            }
        }
        current_max += v;
    }

    long long best_window_val = 0;
    for (int val = 0; val <= max_window_val; val++) {
        if (dp[val] <= rem_T) {
            if (val > best_window_val) {
                best_window_val = val;
            }
        }
    }

    cout << base_val + best_window_val << "\n";
    return 0;
}
