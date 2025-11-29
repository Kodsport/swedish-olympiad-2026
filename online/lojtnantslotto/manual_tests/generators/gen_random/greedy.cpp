#include <bits/stdc++.h>
using namespace std;

struct Castle {
    long long l, g;
};

long long greedy_take(const vector<Castle>& castles, long long T) {
    vector<Castle> arr = castles;

    // sort by (l/g ratio, then l, then -g), all descending
    sort(arr.begin(), arr.end(), [](const Castle& a, const Castle& b) {
        long double ra = (long double)a.l / a.g;
        long double rb = (long double)b.l / b.g;
        if (ra != rb) return ra > rb;
        if (a.l != b.l) return a.l > b.l;
        return a.g < b.g; // since -g descending → g ascending
    });

    long long troops = T;
    long long val = 0;
    for (auto &c : arr) {
        if (c.g <= troops) {
            troops -= c.g;
            val += c.l;
        }
    }
    return val;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N, T;
    cin >> N >> T;

    vector<Castle> castles;
    castles.reserve(N);

    for (int i = 0; i < N; i++) {
        long long l, g;
        cin >> l >> g;
        if (g <= T) {
            castles.push_back({l, g});
        }
    }

    if (castles.empty()) {
        cout << 0 << "\n";
        return 0;
    }

    long long best = greedy_take(castles, T);

    cout << best << "\n";
    return 0;
}
