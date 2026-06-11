#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct Item {
    long long v, w;
};

// Sort by value/weight efficiency descending.
// Break ties by favoring larger absolute values.
bool compare_items(const Item& a, const Item& b) {
    long long lhs = a.v * b.w;
    long long rhs = b.v * a.w;
    if (lhs != rhs) return lhs > rhs;
    return a.v > b.v;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_input;
    long long T;
    if (!(cin >> n_input >> T)) return 0;

    // Freqency map to group identical items
    map<pair<long long, long long>, int> counts;
    for (int i = 0; i < n_input; ++i) {
        long long v, w;
        cin >> v >> w;
        counts[{v, w}]++;
    }

    // Binary Splitting: Break identical occurrences into powers of 2 (1, 2, 4, 8 ...)
    // to bypass combinatorial branching explosions. Keep only chunks strictly <= T.
    vector<Item> items;
    for (auto& kv : counts) {
        long long v = kv.first.first;
        long long w = kv.first.second;
        int c = kv.second;

        int k = 1;
        while (c >= k) {
            if (w * k <= T) {
                items.push_back({v * k, w * k});
            }
            c -= k;
            k *= 2;
        }
        if (c > 0) {
            if (w * c <= T) {
                items.push_back({v * c, w * c});
            }
        }
    }

    sort(items.begin(), items.end(), compare_items);

    int N = items.size();
    vector<long long> pref_w(N + 1, 0);
    vector<long long> pref_v(N + 1, 0);
    for (int i = 0; i < N; ++i) {
        pref_w[i + 1] = pref_w[i] + items[i].w;
        pref_v[i + 1] = pref_v[i] + items[i].v;
    }

    long long max_val = 0;
    int steps = 0;
    const int MAX_STEPS = 5000000;

    // Dantzig's Bound: Get max valid fraction of remaining capacity
    auto get_ub_int = [&](int idx, long long rem_w) -> long long {
        if (idx >= N) return 0;
        long long target = pref_w[idx] + rem_w;
        
        // Find largest stretch of entire items that fit sequentially into bounds
        auto it = std::upper_bound(pref_w.begin() + idx, pref_w.end(), target);
        int ans = std::distance(pref_w.begin(), it) - 2;

        long long ub = pref_v[ans + 1] - pref_v[idx];
        long long current_rem = rem_w - (pref_w[ans + 1] - pref_w[idx]);
        
        // Fractional split mapping over boundary constraints 
        if (ans + 1 < N) {
            ub += (current_rem * items[ans + 1].v) / items[ans + 1].w;
        }
        return ub;
    };

    // DFS with branch and bound and an iteration tripwire (heuristical fallover limit)
    auto dfs = [&](auto& self, int idx, long long cur_w, long long cur_v) -> void {
        if (steps++ > MAX_STEPS) return;
        if (cur_v > max_val) max_val = cur_v;
        if (idx == N) return;

        // B&B Prune condition check
        if (cur_v + get_ub_int(idx, T - cur_w) <= max_val) return;

        // Try taking item optimally first (Greedy approach dictates the priority hierarchy)
        if (cur_w + items[idx].w <= T) {
            self(self, idx + 1, cur_w + items[idx].w, cur_v + items[idx].v);
        }
        
        if (steps > MAX_STEPS) return;

        // Try skipping the item
        self(self, idx + 1, cur_w, cur_v);
    };

    dfs(dfs, 0, 0, 0);

    cout << max_val << "\n";
    return 0;
}
