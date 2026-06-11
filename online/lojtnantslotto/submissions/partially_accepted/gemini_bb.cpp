#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int v;
    int w;
};

int N;
long long T_cap;
vector<Item> items;
vector<int> weights[6];
vector<long long> P_arr[6];
int n_arr[6];

long long best_val = 0;

void search_around(int slack, int W, const vector<int>& center) {
    vector<int> types;
    for(int i = 1; i <= 5; ++i) {
        if(i != slack) types.push_back(i);
    }
    
    // Sort descending to place heavier impact loops outward
    sort(types.begin(), types.end(), [](int a, int b) {
        return a > b;
    });
    
    int t1 = types[0], t2 = types[1], t3 = types[2], t4 = types[3];
    
    int start1 = max(0, center[t1] - W), end1 = min(n_arr[t1], center[t1] + W);
    int start2 = max(0, center[t2] - W), end2 = min(n_arr[t2], center[t2] + W);
    int start3 = max(0, center[t3] - W), end3 = min(n_arr[t3], center[t3] + W);
    int start4 = max(0, center[t4] - W), end4 = min(n_arr[t4], center[t4] + W);
    
    for(int c1 = start1; c1 <= end1; ++c1) {
        long long rem1 = T_cap - P_arr[t1][c1];
        if(rem1 < 0) continue;
        for(int c2 = start2; c2 <= end2; ++c2) {
            long long rem2 = rem1 - P_arr[t2][c2];
            if(rem2 < 0) continue;
            for(int c3 = start3; c3 <= end3; ++c3) {
                long long rem3 = rem2 - P_arr[t3][c3];
                if(rem3 < 0) continue;
                
                int cur_start4 = start4;
                int cur_end4 = min((long long)end4, (long long)(upper_bound(P_arr[t4].begin(), P_arr[t4].end(), rem3) - P_arr[t4].begin() - 1));
                if(cur_start4 > cur_end4) continue;
                
                int c_slack = upper_bound(P_arr[slack].begin(), P_arr[slack].end(), rem3 - P_arr[t4][cur_start4]) - P_arr[slack].begin() - 1;
                
                for(int c4 = cur_start4; c4 <= cur_end4; ++c4) {
                    long long rem4 = rem3 - P_arr[t4][c4];
                    while(c_slack > 0 && P_arr[slack][c_slack] > rem4) {
                        c_slack--;
                    }
                    long long val = c1 * 1LL * t1 + c2 * 1LL * t2 + c3 * 1LL * t3 + c4 * 1LL * t4 + c_slack * 1LL * slack;
                    if(val > best_val) best_val = val;
                }
            }
        }
    }
}

long long get_iters(int W, const vector<vector<int>>& unique_centers) {
    long long total = 0;
    for(const auto& center : unique_centers) {
        for(int slack = 1; slack <= 5; ++slack) {
            vector<int> types;
            for(int i = 1; i <= 5; ++i) {
                if(i != slack) types.push_back(i);
            }
            long long iters = 1;
            for(int t : types) {
                int s = max(0, center[t] - W);
                int e = min(n_arr[t], center[t] + W);
                if (s > e) { iters = 0; break; }
                long long ways = (e - s + 1);
                
                if (2000000000000000000LL / iters < ways) {
                    iters = 2000000000000000000LL;
                } else {
                    iters *= ways;
                }
            }
            total += iters;
            if (total > 2000000000000000000LL) total = 2000000000000000000LL;
        }
    }
    return total;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N_in;
    long long T_in;
    if (!(cin >> N_in >> T_in)) return 0;
    T_cap = T_in;
    
    for (int i = 0; i < N_in; ++i) {
        int v;
        long long w;
        cin >> v >> w;
        if (w <= T_cap) {
            items.push_back({v, (int)w});
            weights[v].push_back((int)w);
        }
    }
    N = items.size();
    if (N == 0) {
        cout << 0 << "\n";
        return 0;
    }
    
    // DP Bypass optimization guaranteeing 100% accurate results whenever dynamically available
    if (1LL * N * T_cap <= 100000000LL) {
        vector<int> dp(T_cap + 1, 0);
        long long max_w = 0;
        for(auto& item : items) {
            long long limit = min((long long)T_cap, max_w + item.w);
            for(long long w = limit; w >= item.w; --w) {
                if (dp[w] < dp[w - item.w] + item.v) {
                    dp[w] = dp[w - item.w] + item.v;
                }
            }
            max_w = limit;
        }
        int ans = 0;
        for(long long w = 0; w <= T_cap; ++w) {
            if(dp[w] > ans) ans = dp[w];
        }
        cout << ans << "\n";
        return 0;
    }

    for (int v = 1; v <= 5; ++v) {
        sort(weights[v].begin(), weights[v].end());
        n_arr[v] = weights[v].size();
        P_arr[v].assign(n_arr[v] + 1, 0);
        for (int i = 0; i < n_arr[v]; ++i) {
            P_arr[v][i+1] = P_arr[v][i] + weights[v][i];
        }
    }
    
    vector<vector<int>> unique_centers;
    auto add_center = [&](const vector<Item>& sorted_items) {
        vector<int> center(6, 0);
        long long cur_W = 0;
        for(const auto& it : sorted_items) {
            if(cur_W + it.w <= T_cap) {
                cur_W += it.w;
                center[it.v]++;
            }
        }
        unique_centers.push_back(center);
    };

    // Knapsack Greedy State Approaches
    vector<Item> tmp_items = items;
    
    // Center 1: Density
    sort(tmp_items.begin(), tmp_items.end(), [](const Item& a, const Item& b) {
        if (a.v * 1LL * b.w != b.v * 1LL * a.w) return a.v * 1LL * b.w > b.v * 1LL * a.w;
        return a.w < b.w;
    });
    add_center(tmp_items);

    // Center 2: Value priority
    sort(tmp_items.begin(), tmp_items.end(), [](const Item& a, const Item& b) {
        if (a.v != b.v) return a.v > b.v;
        return a.w < b.w;
    });
    add_center(tmp_items);

    // Center 3: Weight priority
    sort(tmp_items.begin(), tmp_items.end(), [](const Item& a, const Item& b) {
        if (a.w != b.w) return a.w < b.w;
        return a.v > b.v;
    });
    add_center(tmp_items);

    sort(unique_centers.begin(), unique_centers.end());
    unique_centers.erase(unique(unique_centers.begin(), unique_centers.end()), unique_centers.end());
    
    // Dynamically calculate our multi-dimensional grid size
    int low = 0, high = 200000, best_W = 0;
    while(low <= high) {
        int mid = low + (high - low) / 2;
        if(get_iters(mid, unique_centers) <= 200000000LL) {
            best_W = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    for(const auto& center : unique_centers) {
        for(int slack = 1; slack <= 5; ++slack) {
            search_around(slack, best_W, center);
        }
    }
    
    cout << best_val << "\n";
    return 0;
}