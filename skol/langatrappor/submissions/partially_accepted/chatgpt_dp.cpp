#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (ll)9e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll R; int C;
    if (!(cin >> R >> C)) return 0;
    vector<ll> f(C);
    for (int i = 0; i < C; ++i) cin >> f[i];
    int s_col; ll s_row;
    int g_col; ll g_row;
    cin >> s_col >> s_row;
    cin >> g_col >> g_row;

    // Interpret input: columns in [0..C-1], rows in [0..R-1].
    // We want start_row <= goal_row (we will swap if needed).
    bool swapped = false;
    if (s_row > g_row) {
        swap(s_col, g_col);
        swap(s_row, g_row);
        swapped = true;
    }

    auto h = [&](ll row, int col)->ll {
        // height at (row, col)
        return f[col] - row;
    };

    // Arrays for ascent DP (A_prev -> A_cur) and descent DP (D_prev -> D_cur)
    vector<ll> A_prev(C, INF), A_cur(C, INF);
    vector<ll> D_prev(C, INF), D_cur(C, INF);

    auto relax_row = [&](vector<ll>& enter, ll row) {
        // in-place relax on enter[], treating enter[j] as cost to "enter" column j
        // and allowing further horizontal moves in this row (each step adds h(row, newcol))
        // do left->right
        for (int j = 1; j < C; ++j) {
            if (enter[j-1] != INF) {
                ll cand = enter[j-1] + h(row, j);
                if (cand < enter[j]) enter[j] = cand;
            }
        }
        // right->left
        for (int j = C-2; j >= 0; --j) {
            if (enter[j+1] != INF) {
                ll cand = enter[j+1] + h(row, j);
                if (cand < enter[j]) enter[j] = cand;
            }
        }
    };

    // Prepare to sweep rows from min_row = min(s_row, g_row) up to R-1
    ll min_row = min(s_row, g_row);
    ll max_row = R - 1;

    // Initialize A_prev when we reach s_row; before that A_prev is undefined.
    // Initialize D_prev when we reach g_row.

    // To coordinate, we'll iterate row = min_row .. max_row,
    // and at each row compute A_cur if row >= s_row and D_cur if row >= g_row.
    ll answer = INF;

    for (ll row = min_row; row <= max_row; ++row) {
        // compute A_cur if we have reached s_row
        if (row == s_row) {
            // base: only start_col is reachable by staying on same row initially
            vector<ll> enter(C, INF);
            enter[s_col] = h(row, s_col);
            relax_row(enter, row);
            A_cur = enter;
        } else if (row > s_row) {
            // enter from A_prev by stepping up in same column
            vector<ll> enter(C, INF);
            for (int j = 0; j < C; ++j) {
                if (A_prev[j] != INF) enter[j] = A_prev[j] + h(row, j);
            }
            relax_row(enter, row);
            A_cur = enter;
        }

        // compute D_cur if we have reached g_row (D is cost from (row, col) to goal)
        if (row == g_row) {
            vector<ll> enter(C, INF);
            enter[g_col] = h(row, g_col);
            relax_row(enter, row);
            D_cur = enter;
        } else if (row > g_row) {
            vector<ll> enter(C, INF);
            for (int j = 0; j < C; ++j) {
                if (D_prev[j] != INF) enter[j] = D_prev[j] + h(row, j);
            }
            relax_row(enter, row);
            D_cur = enter;
        }

        // If both A and D are available at this row, combine to get candidate
        if (row >= s_row && row >= g_row) {
            // prefix sums of h(row, j)
            vector<ll> P(C+1, 0);
            for (int j = 0; j < C; ++j) P[j+1] = P[j] + h(row, j);

            // left-to-right: handle y <= z
            ll bestLeft = INF; // min_{y<=z} (A[y] - P[y+1])
            for (int z = 0; z < C; ++z) {
                // update bestLeft with y = z (so it becomes min_{y<=z} after update)
                ll val = (A_cur[z] == INF ? INF : A_cur[z] - P[z+1]);
                if (val < bestLeft) bestLeft = val;
                if (D_cur[z] != INF && bestLeft != INF) {
                    ll cand = bestLeft + (D_cur[z] + P[z]);
                    if (cand < answer) answer = cand;
                }
            }

            // right-to-left: handle y >= z
            ll bestRight = INF; // min_{y>=z} (A[y] + P[y])
            for (int z = C-1; z >= 0; --z) {
                ll val = (A_cur[z] == INF ? INF : A_cur[z] + P[z]);
                if (val < bestRight) bestRight = val;
                if (D_cur[z] != INF && bestRight != INF) {
                    ll cand = bestRight + (D_cur[z] - P[z+1]);
                    if (cand < answer) answer = cand;
                }
            }
        }

        // move A_cur -> A_prev and D_cur -> D_prev for next row iteration
        if (row >= s_row) A_prev.swap(A_cur);
        if (row >= g_row) D_prev.swap(D_cur);
    }

    cout << answer << '\n';
    return 0;
}
