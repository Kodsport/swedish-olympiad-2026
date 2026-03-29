// Random + heuristic sampling of rotations with O(N) stack matching per query.
// For small N, tries all rotations. For large N, uses smart candidates + random
// sampling with local search until near the time limit.
#include <bits/stdc++.h>
using namespace std;

int n;
long long a, b;
char buf[500001];

inline long long cost(int r) {
    int stk = 0, unmatched = 0;
    for (int i = r; i < n; i++) {
        if (buf[i] == '(') stk++;
        else if (stk > 0) stk--;
        else unmatched++;
    }
    for (int i = 0; i < r; i++) {
        if (buf[i] == '(') stk++;
        else if (stk > 0) stk--;
        else unmatched++;
    }
    unmatched += stk;
    return b * r + a * unmatched;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> a >> b;
    string s;
    cin >> s;
    memcpy(buf, s.c_str(), n);

    long long best = LLONG_MAX;
    int best_r = 0;
    auto tryR = [&](int r) {
        r = ((r % n) + n) % n;
        long long c = cost(r);
        if (c < best) { best = c; best_r = r; }
    };

    // For small N: brute force all rotations
    if ((long long)n * n <= 5000000LL) {
        for (int r = 0; r < n; r++) tryR(r);
        cout << best << "\n";
        return 0;
    }

    auto start = chrono::steady_clock::now();
    auto elapsed_ms = [&]() -> int {
        return chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - start).count();
    };
    const int TIME_LIMIT_MS = 750;

    // Phase 1: Collect smart candidates (O(N) scan, no cost() calls)
    vector<int> candidates;
    candidates.push_back(0);
    {
        int psum = 0, mn = 0, mx = 0;
        int argmin_r = 0, argmax_r = 0;
        for (int i = 0; i < n; i++) {
            psum += (buf[i] == '(' ? 1 : -1);
            if (psum < mn) { mn = psum; argmin_r = (i + 1) % n; }
            if (psum > mx) { mx = psum; argmax_r = (i + 1) % n; }
        }
        candidates.push_back(argmin_r);
        candidates.push_back(argmax_r);

        // Evenly spaced
        int step = max(1, n / 30);
        for (int r = 0; r < n; r += step) candidates.push_back(r);
    }

    // Phase 2: Evaluate smart candidates + local search around each
    for (int r : candidates) tryR(r);
    for (int d = 1; d <= 30 && d < n; d++) {
        tryR(best_r + d);
        tryR(best_r - d);
    }

    if (elapsed_ms() > TIME_LIMIT_MS) { cout << best << "\n"; return 0; }

    // Phase 3: Random + local search
    mt19937 rng(12345);
    int qcount = 0;
    while (true) {
        int r = rng() % n;
        long long c = cost(r);
        if (c < best) {
            best = c;
            best_r = r;
            for (int d = 1; d <= 10 && d < n; d++) {
                tryR(best_r + d);
                tryR(best_r - d);
            }
        }
        if (++qcount >= 4) {
            qcount = 0;
            if (elapsed_ms() > TIME_LIMIT_MS) break;
        }
    }

    cout << best << "\n";
}
