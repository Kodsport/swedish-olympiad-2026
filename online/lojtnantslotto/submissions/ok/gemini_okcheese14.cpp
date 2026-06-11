// gemini_cheese14.cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

// Representation of each item (castle)
struct Item {
    int v;
    long long g;
    int id;
    
    // Sort efficiency by value / weight descending
    bool operator<(const Item& o) const {
        if (g * o.v != o.g * v) {
            return g * o.v < o.g * v;
        }
        return v > o.v; // In case of efficiency ties, prefer larger payloads
    }
};

int main() {
    // Optimize IO operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;

    vector<long long> groups[6];
    vector<Item> all_items;
    all_items.reserve(N);

    for (int i = 0; i < N; ++i) {
        int l;
        long long g;
        cin >> l >> g;
        groups[l].push_back(g);
        all_items.push_back({l, g, i});
    }

    // Sort subsets to strictly take the smallest weight prefixes
    for (int i = 1; i <= 5; ++i) {
        if (!groups[i].empty()) {
            sort(groups[i].begin(), groups[i].end());
        }
    }

    sort(all_items.begin(), all_items.end());

    vector<int> G(6, 0);
    long long current_weight = 0;
    long long current_value = 0;

    // Apply baseline Greedy Strategy
    for (const auto& item : all_items) {
        if (current_weight + item.g <= T) {
            current_weight += item.g;
            current_value += item.v;
            G[item.v]++;
        }
    }

    vector<vector<long long>> pref(6);
    for (int i = 1; i <= 5; ++i) {
        pref[i].assign(groups[i].size() + 1, 0);
        for (size_t j = 0; j < groups[i].size(); ++j) {
            pref[i][j + 1] = pref[i][j] + groups[i][j];
        }
    }

    // Determine the maximum boundary 'M' allowed under operational limits (approx < 5e8 operations)
    int M = 0;
    while (M < N) {
        int next_M = M + 1;
        long long max_pos_d = 0, max_neg_d = 0;
        
        for (int i = 1; i <= 5; ++i) {
            max_pos_d += i * min(next_M, (int)groups[i].size() - G[i]);
            max_neg_d += i * min(next_M, G[i]);
        }
        
        long long total_states = max_pos_d + max_neg_d + 1;
        long long ops = 0;
        
        for (int i = 1; i <= 5; ++i) {
            if (groups[i].empty()) continue;
            int min_dx = max(-next_M, -G[i]);
            int max_dx = min(next_M, (int)groups[i].size() - G[i]);
            ops += total_states * (max_dx - min_dx + 1);
        }
        
        if (ops > 5e8) {
            break;
        }
        M = next_M;
    }

    if (M == 0) {
        cout << current_value << "\n";
        return 0;
    }

    long long max_pos_d = 0, max_neg_d = 0;
    for (int i = 1; i <= 5; ++i) {
        max_pos_d += i * min(M, (int)groups[i].size() - G[i]);
        max_neg_d += i * min(M, G[i]);
    }

    int TOTAL_STATES = max_pos_d + max_neg_d + 1;
    int OFFSET = max_neg_d;

    vector<long long> prev_dp(TOTAL_STATES, INF);
    vector<long long> curr_dp(TOTAL_STATES, INF);
    prev_dp[OFFSET] = 0;

    // Search around Greedy Boundary with window `M` per dimension
    for (int i = 1; i <= 5; ++i) {
        if (groups[i].empty()) continue;

        int C_i = groups[i].size();
        int min_dx = max(-M, -G[i]);
        int max_dx = min(M, C_i - G[i]);
        long long base_pref = pref[i][G[i]];

        fill(curr_dp.begin(), curr_dp.end(), INF);

        for (int d = 0; d < TOTAL_STATES; ++d) {
            long long pdp = prev_dp[d];
            if (pdp == INF) continue;

            for (int dx = min_dx; dx <= max_dx; ++dx) {
                int nd = d + dx * i;
                long long cost = pdp + pref[i][G[i] + dx] - base_pref;
                if (cost < curr_dp[nd]) {
                    curr_dp[nd] = cost;
                }
            }
        }
        prev_dp = curr_dp;
    }

    long long max_val = 0;
    for (int d = 0; d < TOTAL_STATES; ++d) {
        if (prev_dp[d] != INF) {
            long long total_weight = current_weight + prev_dp[d];
            if (total_weight <= T) {
                long long total_val = current_value + (d - OFFSET);
                if (total_val > max_val) {
                    max_val = total_val;
                }
            }
        }
    }

    cout << max_val << "\n";
    return 0;
}
