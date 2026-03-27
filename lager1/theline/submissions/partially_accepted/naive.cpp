#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, k;
    if (!(cin >> n >> k)) return 0;
    
    vector<pair<int, int>> pairs(k);
    long long orig_sum = 0;
    for (int i = 0; i < k; ++i) {
        cin >> pairs[i].first >> pairs[i].second;
        orig_sum += abs(pairs[i].first - pairs[i].second);
    }

    long long min_sum = orig_sum;

    for (int u = 1; u <= n; ++u) {
        for (int v = u + 1; v <= n; ++v) {
            long long current_sum = 0;
            for (int i = 0; i < k; ++i) {
                int s = pairs[i].first;
                int t = pairs[i].second;
                int d0 = abs(s - t);
                int d1 = abs(s - u) + 1 + abs(t - v);
                int d2 = abs(s - v) + 1 + abs(t - u);
                int best_d = d0;
                if (d1 < best_d) best_d = d1;
                if (d2 < best_d) best_d = d2;
                current_sum += best_d;
            }
            if (current_sum < min_sum) {
                min_sum = current_sum;
            }
        }
    }

    cout << min_sum << "\n";
    return 0;
}
