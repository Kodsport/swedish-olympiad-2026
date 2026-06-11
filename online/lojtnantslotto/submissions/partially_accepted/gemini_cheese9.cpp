#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    long long l, g;
    int id;
};

// Sort by value/weight descending. Tie break with larger value, then smaller weight.
bool compareItems(const Item& a, const Item& b) {
    if (a.l * b.g != b.l * a.g) {
        return a.l * b.g > b.l * a.g;
    }
    if (a.l != b.l) return a.l > b.l;
    if (a.g != b.g) return a.g < b.g;
    return a.id < b.id;
}

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;
    
    vector<Item> items(N);
    for (int i = 0; i < N; ++i) {
        cin >> items[i].l >> items[i].g;
        items[i].id = i;
    }
    
    sort(items.begin(), items.end(), compareItems);
    
    long long current_weight = 0;
    long long current_value = 0;
    int break_item = N;
    
    // Greedily take items until we reach capacity
    for (int i = 0; i < N; ++i) {
        if (current_weight + items[i].g > T) {
            break_item = i;
            break;
        }
        current_weight += items[i].g;
        current_value += items[i].l;
    }
    
    // If we can afford all castles
    if (break_item == N) {
        cout << current_value << "\n";
        return 0;
    }
    
    // Core limit parameters (Adjust B to strictly control exactly when you want the heuristic to fail)
    int B = 3000;
    if (N <= 6000) {
        B = N; // Converts heuristic into EXACT solution for N <= 6000
    }
    
    int L = max(0, break_item - B);
    int R = min(N - 1, break_item + B);
    
    long long W_base = 0;
    long long V_base = 0;
    
    // Lock in everything before the 'L' window radius
    for (int i = 0; i < L; ++i) {
        W_base += items[i].g;
        V_base += items[i].l;
    }
    
    long long T_prime = T - W_base;
    int max_val = 0;
    for (int i = L; i <= R; ++i) {
        max_val += items[i].l;
    }
    
    const long long INF = 1e18;
    vector<long long> dp(max_val + 1, INF);
    dp[0] = 0;
    
    int current_max_v = 0;
    // Window DP over items strictly within bounds
    for (int i = L; i <= R; ++i) {
        int v = items[i].l;
        long long w = items[i].g;
        for (int j = current_max_v; j >= 0; --j) {
            long long dp_j = dp[j];
            if (dp_j != INF && dp_j + w <= T_prime) {
                if (dp_j + w < dp[j + v]) {
                    dp[j + v] = dp_j + w;
                }
            }
        }
        current_max_v += v;
    }
    
    long long best_extra = 0;
    for (int j = 0; j <= max_val; ++j) {
        if (dp[j] <= T_prime) {
            best_extra = max(best_extra, (long long)j);
        }
    }
    
    cout << V_base + best_extra << "\n";
    return 0;
}
