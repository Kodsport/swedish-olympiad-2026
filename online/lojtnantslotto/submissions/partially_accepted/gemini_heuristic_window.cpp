#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Item {
    long long l, g;
    int id;
    
    // Sort primarily by density (l/g) descending. 
    // Tie-break by smaller weight ascending for better item combinatorics 
    bool operator<(const Item& other) const {
        if (l * other.g != other.l * g) {
            return l * other.g > other.l * g;
        }
        return g < other.g;
    }
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long T;
    if (!(cin >> n >> T)) return 0;

    vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        cin >> items[i].l >> items[i].g;
        items[i].id = i;
    }

    sort(items.begin(), items.end());

    long long current_weight = 0;
    long long current_value = 0;
    vector<int> taken_indices;
    vector<int> not_taken_indices;

    // 1. Initial greedy allocation sequence
    for (int i = 0; i < n; ++i) {
        if (current_weight + items[i].g <= T) {
            current_weight += items[i].g;
            current_value += items[i].l;
            taken_indices.push_back(i);
        } else {
            not_taken_indices.push_back(i);
        }
    }

    // 2. Identify the DP bounding area
    int W_left = 4000;
    int W_right = 4000;

    int L_taken = max(0, (int)taken_indices.size() - W_left);
    int R_not_taken = min((int)not_taken_indices.size(), W_right);

    long long base_weight = 0;
    long long base_value = 0;

    for (int i = 0; i < L_taken; ++i) {
        base_weight += items[taken_indices[i]].g;
        base_value += items[taken_indices[i]].l;
    }

    // Pool surrounding items for the exact knapsack pass
    vector<Item> dp_items;
    dp_items.reserve(W_left + W_right);
    for (int i = L_taken; i < taken_indices.size(); ++i) {
        dp_items.push_back(items[taken_indices[i]]);
    }
    for (int i = 0; i < R_not_taken; ++i) {
        dp_items.push_back(items[not_taken_indices[i]]);
    }

    int total_v = 0;
    for (const auto& item : dp_items) {
        total_v += item.l;
    }

    // 3. Local DP subset Knapsack (Bounded)
    vector<long long> dp(total_v + 1, INF);
    dp[0] = 0;

    int current_max_v = 0;
    for (const auto& item : dp_items) {
        long long w = item.g;
        int v = item.l;
        for (int j = current_max_v; j >= 0; --j) {
            if (dp[j] != INF) {
                long long new_w = dp[j] + w;
                if (new_w < dp[j + v]) {
                    dp[j + v] = new_w;
                }
            }
        }
        current_max_v += v;
    }

    long long max_ans = 0;
    for (int j = 0; j <= total_v; ++j) {
        if (dp[j] <= T - base_weight) {
            max_ans = max(max_ans, base_value + j);
        }
    }

    cout << max_ans << "\n";

    return 0;
}
