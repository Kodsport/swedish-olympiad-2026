// gemini_cheese10.cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    long long l, g;
};

// Sort by efficiency (l / g) descending. On tie, prioritize larger value.
bool cmp(const Item& a, const Item& b) {
    if (a.l * b.g != b.l * a.g) return a.l * b.g > b.l * a.g;
    if (a.l != b.l) return a.l > b.l;
    return a.g < b.g;
}

int main() {
    // Optimize standard I/O operations for speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;
    
    vector<Item> items(N);
    vector<vector<long long>> type_items(6);
    
    for (int i = 0; i < N; ++i) {
        cin >> items[i].l >> items[i].g;
        type_items[items[i].l].push_back(items[i].g);
    }
    
    vector<int> N_type(6, 0);
    vector<vector<long long>> pref(6);
    
    for (int i = 1; i <= 5; ++i) {
        sort(type_items[i].begin(), type_items[i].end());
        N_type[i] = type_items[i].size();
        pref[i].assign(N_type[i] + 1, 0);
        for (int j = 0; j < N_type[i]; ++j) {
            pref[i][j + 1] = pref[i][j] + type_items[i][j];
        }
    }
    
    // Process the standard Greedy Fractional approach as our heuristic center
    vector<Item> all_items = items;
    sort(all_items.begin(), all_items.end(), cmp);
    
    vector<int> G(6, 0);
    long long current_weight = 0;
    
    for (int i = 0; i < N; ++i) {
        if (current_weight + all_items[i].g <= T) {
            current_weight += all_items[i].g;
            G[all_items[i].l]++;
        }
    }
    
    // Choose the variable with the most items as the dependent nested target 
    // to dynamically fill up all remaining knapsack space unconditionally
    int dep = 1;
    for (int i = 2; i <= 5; ++i) {
        if (N_type[i] > N_type[dep]) {
            dep = i;
        }
    }
    
    vector<int> t;
    for (int i = 1; i <= 5; ++i) {
        if (i != dep) t.push_back(i);
    }
    
    int t0 = t[0], t1 = t[1], t2 = t[2], t3 = t[3];
    long long max_val = 0;
    int D = 45; // Local window limit. Evaluates a maximum neighborhood difference up to D = 45.
    
    int start0 = max(0, G[t0] - D), end0 = min(N_type[t0], G[t0] + D);
    int start1 = max(0, G[t1] - D), end1 = min(N_type[t1], G[t1] + D);
    int start2 = max(0, G[t2] - D), end2 = min(N_type[t2], G[t2] + D);
    int start3 = max(0, G[t3] - D), end3 = min(N_type[t3], G[t3] + D);
    
    // Search the multi-dimensional local area bounding box
    for (int c0 = start0; c0 <= end0; ++c0) {
        long long w0 = pref[t0][c0];
        for (int c1 = start1; c1 <= end1; ++c1) {
            long long w1 = pref[t1][c1];
            for (int c2 = start2; c2 <= end2; ++c2) {
                long long w2 = pref[t2][c2];
                long long rem = T - w0 - w1 - w2;
                if (rem < 0) continue;
                
                int c_dep = -1;
                for (int c3 = start3; c3 <= end3; ++c3) {
                    long long w3 = pref[t3][c3];
                    if (w3 > rem) break; // w3 intrinsically increases monotonically
                    
                    long long rem_dep = rem - w3;
                    
                    if (c_dep == -1) {
                        // Uninitialized inner loop gets evaluated once via robust binary search overhead $O(\log N)$
                        int low = 0, high = N_type[dep];
                        while (low <= high) {
                            int mid = low + (high - low) / 2;
                            if (pref[dep][mid] <= rem_dep) {
                                c_dep = mid;
                                low = mid + 1;
                            } else {
                                high = mid - 1;
                            }
                        }
                    } else {
                        // Use a sliding continuous two-pointers equivalent optimization:
                        // With an increasing `c3`, `rem_dep` will purely decrease therefore reducing `c_dep`.
                        while (c_dep > 0 && pref[dep][c_dep] > rem_dep) {
                            c_dep--;
                        }
                    }
                    
                    long long current_val = 1LL * c0 * t0 + 1LL * c1 * t1 + 1LL * c2 * t2 + 1LL * c3 * t3 + 1LL * c_dep * dep;
                    if (current_val > max_val) {
                        max_val = current_val;
                    }
                }
            }
        }
    }
    
    cout << max_val << "\n";
    return 0;
}