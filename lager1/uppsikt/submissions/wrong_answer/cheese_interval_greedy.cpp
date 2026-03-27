// @EXPECTED_GRADES@ WA WA WA WA WA
//
// Cheese: Computes farthest visible left/right for each position via the
// Cartesian tree, then treats coverage as a contiguous interval and does
// greedy interval covering. Visibility is NOT contiguous in general, so
// this overclaims coverage and underestimates the answer.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    vector<int> p(n);
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);

    if (n == 1) {
        printf("1\n");
        return 0;
    }

    // Build Cartesian tree (max-heap)
    vector<int> L(n, -1), R(n, -1), par(n, -1);
    vector<int> stk;

    for (int i = 0; i < n; i++) {
        int last = -1;
        while (!stk.empty() && p[stk.back()] < p[i]) {
            last = stk.back();
            stk.pop_back();
        }
        if (!stk.empty()) {
            R[stk.back()] = i;
            par[i] = stk.back();
        }
        if (last != -1) {
            L[i] = last;
            par[last] = i;
        }
        stk.push_back(i);
    }

    // For each node, compute interval [lo, hi]
    // Visibility: parent, left child, right child, i-1, i+1
    vector<pair<int,int>> intervals(n);
    for (int i = 0; i < n; i++) {
        int lo = i, hi = i;
        if (par[i] != -1) {
            lo = min(lo, par[i]);
            hi = max(hi, par[i]);
        }
        if (L[i] != -1) lo = min(lo, L[i]);
        if (R[i] != -1) hi = max(hi, R[i]);
        if (i > 0) lo = min(lo, i - 1);
        if (i < n - 1) hi = max(hi, i + 1);
        intervals[i] = {lo, hi};
    }

    // Sort by left endpoint
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b) {
        return intervals[a].first < intervals[b].first;
    });

    // Greedy interval cover of [0, n-1]
    int answer = 0;
    int covered_until = -1;
    int j = 0;

    while (covered_until < n - 1) {
        int best_right = -1;
        while (j < n && intervals[idx[j]].first <= covered_until + 1) {
            best_right = max(best_right, intervals[idx[j]].second);
            j++;
        }
        if (best_right == -1) break;
        covered_until = best_right;
        answer++;
    }

    printf("%d\n", answer);
    return 0;
}
