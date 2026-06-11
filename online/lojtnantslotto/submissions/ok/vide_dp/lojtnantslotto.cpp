
#include <bits/stdc++.h>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

#define rep(i, lo, hi) for (int i = lo; i < hi; i++)
#define per(i, lo, hi) for (int i = hi - 1; i >= lo; i--)
#define all(c) c.begin(), c.end()

const int INF = int(1e18);

using vi = vector<int>;
using vvi = vector<vi>;
using vb = vector<bool>;
using pi = pair<int, int>;

bool bet(int lo, int x, int hi) {
    return lo < x && x < hi;
}

int main() {
    int n, s;
    cin >> n >> s;

    vvi weights(5);
    rep(i, 0, n) {
        int v, c;
        cin >> v >> c;
        weights[v - 1].push_back(c);
    }
    vvi cum(5);
    rep(i, 0, 5) {
        sort(all(weights[i]));

        int cur = 0;
        rep(j, 0, weights[i].size()) {
            if (cur > s) {
                weights[i].resize(j + 1);
                break;
            }

            cum[i].push_back(cur);
            cur += weights[i][j];
        }

        cum[i].push_back(cur);
    }

    // rep(i, 0, 5) {
    //     cerr << i << ": ";
    //     for (int x : weights[i]) {
    //         cerr << x << " ";
    //     }
    //     cerr << "\n";
    // }

    int gold = 0;
    int used = 0;
    vi is(5, 0);
    while (1) {
        vi pos(5);
        rep(i, 0, 5) {
            // cerr << i << " " << is[i] << " " << cum[i].size() << "\n";
            pos[i] = is[i] + 1 >= cum[i].size()
                         ? INF
                         // : cum[i][is[i] + 1] * (60 / (i + 1));
                         : weights[i][is[i]] * (60 / (i + 1));

            // cerr << pos[i] << " ";
        }
        // cerr << "\n";

        auto it = min_element(all(pos));
        int j = distance(pos.begin(), it);

        if (*it == INF)
            break;

        used += weights[j][is[j]];
        if (used > s) {
            used -= weights[j][is[j]];
            break;
        }

        // fprintf(stderr, "%d %d %d\n", j + 1, is[j], weights[j][is[j]]);
        gold += (j + 1);
        is[j]++;
    }

    cerr << gold << "\n";

    // undo 60 points
    rep(i, 0, 5) {
        rep(j, 0, 60 / (1 + i)) {
            if (is[i] == 0)
                break;

            used -= weights[i][is[i] - 1];
            gold -= (i + 1);
            is[i]--;
        }
    }

    // remove items that we wont have space for
    rep(i, 0, 5) {
        int cur = 0;
        rep(j, is[i], weights[i].size()) {
            if (cur > (s - used)) {
                weights[i].resize(j + 1);
                break;
            }

            cur += weights[i][j];
        }
    }

    // vi f(W);
    // for (int i = 1; i <= n; i++)
    //     for (int j = W; j >= w[i]; j--)
    //         f[j] = max(f[j], f[j - w[i]] + v[i]);
    //

    vi w;
    vi v;
    rep(i, 0, 5) {
        rep(j, is[i], weights[i].size()) {
            w.push_back(weights[i][j]);
            v.push_back(i + 1);
        }
    }

    int stor = max<int>(0, s - used) + 1;
    int items = (int)w.size();
    vi f(stor);

    if (stor) {
        rep(i, 0, items) {
            per(j, w[i], stor) {
                f[j] = max(f[j], f[j - w[i]] + v[i]);
            }
        }
        gold += f.back();
    }

    cout << gold;

    // for (int i = 1; i <= items; i++) for (int j = W; j >= w[i]; j--)
    //     f[j] = max(f[j], f[j - w[i]] + v[i]);
}
