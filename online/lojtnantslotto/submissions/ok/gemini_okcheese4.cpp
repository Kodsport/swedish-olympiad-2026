#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

struct Item {
    long long l, g;
    int id;
    int type;
};

// Sort descending by efficiency. Break ties by picking largest value, then smallest weight.
bool compare(const Item& a, const Item& b) {
    long long left = a.l * b.g;
    long long right = b.l * a.g;
    if (left != right) return left > right;
    if (a.l != b.l) return a.l > b.l;
    return a.g < b.g;
}

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;
    
    vector<Item> all_items(N);
    vector<Item> items[6];
    
    for (int i = 0; i < N; ++i) {
        cin >> all_items[i].l >> all_items[i].g;
        all_items[i].id = i;
        all_items[i].type = all_items[i].l;
        items[all_items[i].type].push_back(all_items[i]);
    }
    
    long long cnt[6];
    vector<long long> pref[6];
    
    // Process prefixes sorted by weight descending internally per value
    for (int v = 1; v <= 5; ++v) {
        cnt[v] = items[v].size();
        sort(items[v].begin(), items[v].end(), [](const Item& a, const Item& b) {
            return a.g < b.g;
        });
        pref[v].assign(cnt[v] + 1, 0);
        for (int i = 0; i < cnt[v]; ++i) {
            pref[v][i + 1] = pref[v][i] + items[v][i].g;
        }
    }
    
    // Obtain robust greedy baseline mathematically
    sort(all_items.begin(), all_items.end(), compare);
    
    long long current_weight = 0;
    long long greedy_val = 0;
    vector<long long> G(6, 0);
    
    for (const auto& item : all_items) {
        if (current_weight + item.g <= T) {
            current_weight += item.g;
            greedy_val += item.l;
            G[item.type]++;
        }
    }
    
    // Dynamically calculate the largest search radius D we can safely perform 
    long long budget = 150000000LL; 
    long long L = 1, R = 200000, ans_D = 65;
    while (L <= R) {
        long long mid = L + (R - L) / 2;
        long double iters = 1.0;
        if (cnt[5] > 0) iters *= min((long long)cnt[5] + 1, 2LL * mid + 1);
        if (cnt[4] > 0) iters *= min((long long)cnt[4] + 1, 2LL * mid + 1);
        if (cnt[3] > 0) iters *= min((long long)cnt[3] + 1, 2LL * mid + 1);
        if (cnt[2] > 0) iters *= min((long long)cnt[2] + 1, 2LL * mid + 1);
        
        if (iters <= budget) {
            ans_D = mid;
            L = mid + 1;
        } else {
            R = mid - 1;
        }
    }
    long long D = max(ans_D, 65LL); // Ensure at least a robust minimal radius coverage
    
    int low5 = (int)max(0LL, G[5] - D);
    int high5 = (int)min((long long)cnt[5], G[5] + D);
    
    int low4 = (int)max(0LL, G[4] - D);
    int high4 = (int)min((long long)cnt[4], G[4] + D);
    
    int low3 = (int)max(0LL, G[3] - D);
    int high3 = (int)min((long long)cnt[3], G[3] + D);
    
    int low2 = (int)max(0LL, G[2] - D);
    int high2 = (int)min((long long)cnt[2], G[2] + D);
    
    // DP Array for remainder items of `value=1` bounded purely in linear $O(T)$
    vector<int> max_k1_arr(T + 1, 0);
    int cur = 0;
    for (int w = 0; w <= T; ++w) {
        while (cur + 1 <= cnt[1] && pref[1][cur + 1] <= w) cur++;
        max_k1_arr[w] = cur;
    }
    
    long long best_val = greedy_val;
    auto start_time = std::chrono::steady_clock::now();
    bool time_out = false;
    
    for (int k5 = low5; k5 <= high5; ++k5) {
        if (time_out) break;
        long long w5 = pref[5][k5];
        if (w5 > T) break;
        long long v5 = k5 * 5LL;
        
        for (int k4 = low4; k4 <= high4; ++k4) {
            // Time cutoff to elegantly handle strongly crafted multi-dimensional bounds bypass (graceful WA fallback)
            auto current_time = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() > 950) {
                time_out = true;
                break;
            }
            
            long long w4 = w5 + pref[4][k4];
            if (w4 > T) break;
            long long v4 = v5 + k4 * 4LL;
            
            for (int k3 = low3; k3 <= high3; ++k3) {
                long long w3 = w4 + pref[3][k3];
                if (w3 > T) break;
                long long v3 = v4 + k3 * 3LL;
                
                for (int k2 = low2; k2 <= high2; ++k2) {
                    long long w2 = w3 + pref[2][k2];
                    if (w2 > T) break; // `pref` arrays naturally strictly scale upwards
                    
                    long long val = v3 + k2 * 2LL + max_k1_arr[T - w2];
                    if (val > best_val) {
                        best_val = val;
                    }
                }
            }
        }
    }
    
    cout << best_val << "\n";
    return 0;
}
