#include <bits/stdc++.h>
using namespace std;

struct Castle {
    long long l;  // value
    long long g;  // cost
};

// The greedy_take function: sort by key desc, then take items with g <= troops
template<typename KeyFunc>
long long greedy_take(const vector<Castle>& castles, long long T, KeyFunc key) {
    vector<Castle> arr = castles;
    sort(arr.begin(), arr.end(),
         [&](const Castle& a, const Castle& b) {
             return key(a) > key(b);  // descending order
         });

    long long troops = T;
    long long val = 0;
    for (auto& c : arr) {
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

    int N;
    long long T;
    if (!(cin >> N >> T)) return 0;

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

    vector<long long> cand;

    // heuristic 1: l/g ratio, break ties by l, then -g
    cand.push_back(greedy_take(castles, T, [&](const Castle& c) {
        return tuple<double,long long,long long>(
            double(c.l) / c.g, c.l, -c.g
        );
    }));

    // heuristic 2: highest l, tie-break smaller g
    cand.push_back(greedy_take(castles, T, [&](const Castle& c) {
        return tuple<long long,long long>(c.l, -c.g);
    }));

    // heuristic 3: lowest cost first (i.e., sort by -g descending)
    cand.push_back(greedy_take(castles, T, [&](const Castle& c) {
        return tuple<long long,long long>(-c.g, c.l);
    }));

    // heuristic 4: l / (1 + log(1 + g))
    cand.push_back(greedy_take(castles, T, [&](const Castle& c) {
        return tuple<double,long long>(
            double(c.l) / (1.0 + log(1.0 + c.g)),
            c.l
        );
    }));

    // heuristic 5: prioritize large l, prefer smaller g among same l
    cand.push_back(greedy_take(castles, T, [&](const Castle& c) {
        return tuple<long long,long long>(c.l, -c.g);
    }));

    // heuristic 6: ratio but prefer smaller g on tie, then larger l
    cand.push_back(greedy_take(castles, T, [&](const Castle& c) {
        return tuple<double,long long,long long>(
            double(c.l) / c.g,
            -c.g,
            c.l
        );
    }));

    cout << *max_element(cand.begin(), cand.end()) << "\n";
    return 0;
}
