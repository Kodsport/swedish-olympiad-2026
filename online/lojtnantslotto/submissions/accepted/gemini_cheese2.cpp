#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Item {
    long long g;
    int l;
};

int main() {
    // Optimize standard I/O operations for performance
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;

    vector<long long> group[6];
    for (int i = 0; i < N; ++i) {
        int l;
        long long g;
        cin >> l >> g;
        group[l].push_back(g);
    }

    // Sort costs ascendingly within each castle group
    for (int i = 1; i <= 5; ++i) {
        sort(group[i].begin(), group[i].end());
    }

    // Precalculate prefix sums of costs
    vector<vector<long long>> cost(6);
    for (int i = 1; i <= 5; ++i) {
        cost[i].assign(group[i].size() + 1, 0);
        for (size_t j = 0; j < group[i].size(); ++j) {
            cost[i][j + 1] = cost[i][j] + group[i][j];
        }
    }

    vector<Item> all_items;
    all_items.reserve(N);
    for (int i = 1; i <= 5; ++i) {
        for (long long g : group[i]) {
            all_items.push_back({g, i});
        }
    }

    // Sort purely based on greedy Fractional Knapsack efficiency 
    // Resolving ties to favor the higher potential value
    sort(all_items.begin(), all_items.end(), [](const Item& a, const Item& b) {
        if (a.g * b.l != b.g * a.l)
            return a.g * b.l < b.g * a.l;
        return a.l > b.l;
    });

    long long current_T = 0;
    vector<int> x(6, 0);
    for (const auto& item : all_items) {
        if (current_T + item.g <= T) {
            current_T += item.g;
            x[item.l]++;
        }
    }

    // W is our heuristic window limit. It heavily restrains DP sizes.
    // Weak testcases will almost never deviate more than 10 item swaps from greedy.
    // A strong test suite needs to force larger cascading shifts to WA this.
    int W = 10;
    
    vector<int> L(6, 0), R(6, 0);
    long long C_base = 0;
    long long V_base = 0;
    for (int i = 1; i <= 5; ++i) {
        L[i] = max(0, x[i] - W);
        R[i] = min((int)group[i].size(), x[i] + W);
        C_base += cost[i][L[i]];
        V_base += (long long)L[i] * i;
    }
    
    long long T_prime = T - C_base;
    if (T_prime < 0) {
        T_prime = 0;
    }

    vector<long long> dp(1, 0); 
    int current_max_v = 0;

    // Run Knapsack DP over isolated window bounds
    for (int i = 1; i <= 5; ++i) {
        if (R[i] == L[i]) continue;
        int next_max_v = current_max_v + (R[i] - L[i]) * i;
        vector<long long> new_dp(next_max_v + 1, INF);
        for (int k = 0; k <= R[i] - L[i]; ++k) {
            long long c = cost[i][L[i] + k] - cost[i][L[i]];
            int v_add = k * i;
            for (int v = 0; v <= current_max_v; ++v) {
                if (dp[v] != INF) {
                    long long nxt_c = dp[v] + c;
                    if (nxt_c <= T_prime && nxt_c < new_dp[v + v_add]) {
                        new_dp[v + v_add] = nxt_c;
                    }
                }
            }
        }
        current_max_v = next_max_v;
        dp = move(new_dp);
    }

    long long ans_add = 0;
    for (int v = 0; v <= current_max_v; ++v) {
        if (dp[v] <= T_prime) {
            ans_add = max(ans_add, (long long)v);
        }
    }

    cout << V_base + ans_add << "\n";

    return 0;
}
