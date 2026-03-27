// @EXPECTED_GRADES@ AC AC AC AC TLE
//
// Cheese: Iterative scan-for-max Cartesian tree build. O(n^2) worst case
// on sorted data, but O(n log n) expected on random data (balanced splits).
// Exploits that group 4 contains only random permutations.

#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;
const int MAXN = 300001;

int p[MAXN], L[MAXN], R[MAXN];
int dp[MAXN][2][2][2][2];

void solve(int root, int n) {
    vector<int> order;
    order.reserve(n);
    vector<int> stk = {root};
    while (!stk.empty()) {
        int v = stk.back(); stk.pop_back();
        order.push_back(v);
        if (L[v] != -1) stk.push_back(L[v]);
        if (R[v] != -1) stk.push_back(R[v]);
    }

    for (int idx = (int)order.size() - 1; idx >= 0; idx--) {
        int node = order[idx];
        int ln = L[node], rn = R[node];

        for (int el = 0; el < 2; el++)
            for (int er = 0; er < 2; er++)
                for (int hl = 0; hl < 2; hl++)
                    for (int hr = 0; hr < 2; hr++)
                        dp[node][el][er][hl][hr] = INF;

        for (int el = 0; el < 2; el++) {
            for (int er = 0; er < 2; er++) {
                // Node is lit
                int cost = 1;
                if (ln != -1) {
                    int best = INF;
                    for (int hl = 0; hl < 2; hl++)
                        for (int hr = 0; hr < 2; hr++)
                            best = min(best, dp[ln][el][1][hl][hr]);
                    cost += best;
                }
                if (rn != -1) {
                    int best = INF;
                    for (int hl = 0; hl < 2; hl++)
                        for (int hr = 0; hr < 2; hr++)
                            best = min(best, dp[rn][1][er][hl][hr]);
                    cost += best;
                }
                dp[node][el][er][1][1] = cost;

                // Node is not lit
                struct State { int cost, hl, hr; };
                State ls[4], rs[4];
                int lc = 0, rc = 0;

                if (ln != -1) {
                    for (int hl = 0; hl < 2; hl++)
                        for (int hr = 0; hr < 2; hr++)
                            ls[lc++] = {dp[ln][el][0][hl][hr], hl, hr};
                } else {
                    ls[lc++] = {0, 0, 0};
                }

                if (rn != -1) {
                    for (int hl = 0; hl < 2; hl++)
                        for (int hr = 0; hr < 2; hr++)
                            rs[rc++] = {dp[rn][0][er][hl][hr], hl, hr};
                } else {
                    rs[rc++] = {0, 0, 0};
                }

                for (int a = 0; a < lc; a++) {
                    for (int b = 0; b < rc; b++) {
                        if (ls[a].cost >= INF || rs[b].cost >= INF) continue;
                        if (!(el || er || ls[a].hr || rs[b].hl)) continue;
                        int ol = (ln != -1) ? ls[a].hl : 0;
                        int or_ = (rn != -1) ? rs[b].hr : 0;
                        dp[node][el][er][ol][or_] = min(
                            dp[node][el][er][ol][or_],
                            ls[a].cost + rs[b].cost);
                    }
                }
            }
        }
    }

    int ans = INF;
    for (int hl = 0; hl < 2; hl++)
        for (int hr = 0; hr < 2; hr++)
            ans = min(ans, dp[root][0][0][hl][hr]);
    printf("%d\n", ans);
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);

    memset(L, -1, sizeof(int) * n);
    memset(R, -1, sizeof(int) * n);

    // Iterative scan-for-max Cartesian tree build
    int root = -1;
    struct Entry { int lo, hi, par, side; };
    vector<Entry> stk = {{0, n - 1, -1, -1}};
    while (!stk.empty()) {
        auto [lo, hi, par, side] = stk.back();
        stk.pop_back();
        if (lo > hi) continue;
        int m = lo;
        for (int i = lo + 1; i <= hi; i++)
            if (p[i] > p[m]) m = i;
        if (par == -1) root = m;
        else if (side == 0) L[par] = m;
        else R[par] = m;
        stk.push_back({m + 1, hi, m, 1});
        stk.push_back({lo, m - 1, m, 0});
    }

    solve(root, n);
    return 0;
}
