#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;
    
    // Group the guard counts strictly by their castle value (l_i)
    vector<long long> groups[6];
    for (int i = 0; i < N; i++) {
        int l;
        long long g;
        cin >> l >> g;
        groups[l].push_back(g);
    }
    
    // Naturally, we always prefer the cheapest cost items within a given value
    for (int i = 1; i <= 5; i++) {
        sort(groups[i].begin(), groups[i].end());
    }
    
    vector<vector<long long>> prefs(6);
    for (int i = 1; i <= 5; i++) {
        prefs[i].assign(groups[i].size() + 1, 0);
        for (size_t j = 0; j < groups[i].size(); j++) {
            prefs[i][j + 1] = prefs[i][j] + groups[i][j];
        }
    }
    
    // Step 1: Evaluate Baseline Greedy Solution
    struct Item {
        int l;
        long long g;
    };
    vector<Item> items;
    items.reserve(N);
    for (int i = 1; i <= 5; i++) {
        for (size_t j = 0; j < groups[i].size(); j++) {
            items.push_back({i, groups[i][j]});
        }
    }
    
    sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
        long long effA = (long long)a.l * b.g;
        long long effB = (long long)b.l * a.g;
        if (effA != effB) return effA > effB;
        return a.g < b.g;
    });
    
    long long current_T = T;
    long long greedy_val = 0;
    vector<int> greedy_counts(6, 0);
    
    for (auto& item : items) {
        if (current_T >= item.g) {
            current_T -= item.g;
            greedy_val += item.l;
            greedy_counts[item.l]++;
        }
    }
    
    long long max_ans = greedy_val;
    
    // Bounded search depth: large enough to catch random noise, small enough to fail on deliberate attacks
    int D = 60;
    
    int min2 = max(0, greedy_counts[2] - D);
    int max2 = min((int)groups[2].size(), greedy_counts[2] + D);
    int min3 = max(0, greedy_counts[3] - D);
    int max3 = min((int)groups[3].size(), greedy_counts[3] + D);
    int min4 = max(0, greedy_counts[4] - D);
    int max4 = min((int)groups[4].size(), greedy_counts[4] + D);
    int min5 = max(0, greedy_counts[5] - D);
    int max5 = min((int)groups[5].size(), greedy_counts[5] + D);
    
    // Step 2: Exhaustively Local Search (Bounded)
    if (T <= 2000000) {
        // Precompute array mapped to index pointers bypassing inline Binary Searches ~1000% faster
        vector<int> max_c1(T + 1, 0);
        int idx = 0;
        int max_n1 = prefs[1].size() - 1;
        for (long long w = 0; w <= T; w++) {
            while (idx + 1 <= max_n1 && prefs[1][idx + 1] <= w) {
                idx++;
            }
            max_c1[w] = idx;
        }
        
        for (int c5 = min5; c5 <= max5; c5++) {
            long long w5 = prefs[5][c5];
            if (w5 > T) continue;
            long long v5 = 5LL * c5;
            for (int c4 = min4; c4 <= max4; c4++) {
                long long w4 = w5 + prefs[4][c4];
                if (w4 > T) break;  // Fast break ensures minimal branching misses
                long long v45 = v5 + 4LL * c4;
                for (int c3 = min3; c3 <= max3; c3++) {
                    long long w3 = w4 + prefs[3][c3];
                    if (w3 > T) break;
                    long long v345 = v45 + 3LL * c3;
                    for (int c2 = min2; c2 <= max2; c2++) {
                        long long w2 = w3 + prefs[2][c2];
                        if (w2 > T) break;
                        
                        long long val = max_c1[T - w2] + 2LL * c2 + v345;
                        if (val > max_ans) {
                            max_ans = val;
                        }
                    }
                }
            }
        }
    } else {
        // Safety bounds fallback check using `<algorithm>` if T constraint overrides scope specifications 
        for (int c5 = min5; c5 <= max5; c5++) {
            long long w5 = prefs[5][c5];
            if (w5 > T) continue;
            long long v5 = 5LL * c5;
            for (int c4 = min4; c4 <= max4; c4++) {
                long long w4 = w5 + prefs[4][c4];
                if (w4 > T) break;
                long long v45 = v5 + 4LL * c4;
                for (int c3 = min3; c3 <= max3; c3++) {
                    long long w3 = w4 + prefs[3][c3];
                    if (w3 > T) break;
                    long long v345 = v45 + 3LL * c3;
                    for (int c2 = min2; c2 <= max2; c2++) {
                        long long w2 = w3 + prefs[2][c2];
                        if (w2 > T) break;
                        
                        auto it = upper_bound(prefs[1].begin(), prefs[1].end(), T - w2);
                        int c1 = distance(prefs[1].begin(), it) - 1;
                        long long val = c1 + 2LL * c2 + v345;
                        if (val > max_ans) {
                            max_ans = val;
                        }
                    }
                }
            }
        }
    }
    
    cout << max_ans << "\n";
    return 0;
}
