// @EXPECTED_GRADES@ WA WA WA WA WA
//
// Cheese: Solves minimum dominating set on the Cartesian tree only,
// ignoring non-tree adjacent edges. Overestimates the answer.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    vector<int> p(n);
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);

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

    int root = -1;
    for (int i = 0; i < n; i++) {
        if (par[i] == -1) { root = i; break; }
    }

    // Pre-order traversal
    vector<int> order;
    order.reserve(n);
    stk.clear();
    stk.push_back(root);
    while (!stk.empty()) {
        int v = stk.back(); stk.pop_back();
        order.push_back(v);
        if (L[v] != -1) stk.push_back(L[v]);
        if (R[v] != -1) stk.push_back(R[v]);
    }

    // 3-state tree DP
    const int INF = 1e9;
    vector<int> dp0(n), dp1(n), dp2(n);

    for (int idx = (int)order.size() - 1; idx >= 0; idx--) {
        int v = order[idx];
        int ch[2], nc = 0;
        if (L[v] != -1) ch[nc++] = L[v];
        if (R[v] != -1) ch[nc++] = R[v];

        if (nc == 0) {
            dp0[v] = 1;
            dp1[v] = INF;
            dp2[v] = 0;
        } else {
            int cost0 = 1, cost2 = 0;
            bool has_sel = false;
            int min_extra = INF;
            for (int j = 0; j < nc; j++) {
                int c = ch[j];
                cost0 += min({dp0[c], dp1[c], dp2[c]});
                int m01 = min(dp0[c], dp1[c]);
                cost2 += m01;
                if (dp0[c] <= dp1[c]) has_sel = true;
                min_extra = min(min_extra, dp0[c] - dp1[c]);
            }
            dp0[v] = cost0;
            dp2[v] = cost2;
            dp1[v] = has_sel ? cost2 : cost2 + min_extra;
        }
    }

    printf("%d\n", min(dp0[root], dp1[root]));
    return 0;
}
