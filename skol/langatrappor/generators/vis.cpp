#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<ll>;
using vvi = vector<vi>;
using p2 = pair<ll, ll>;
const ll inf = 1e18;

#define rep(i,n) for (ll i = 0; i < (n); i++)
#define repp(i,j,n) for (ll i = j; i < (n); i++)
#define repe(i, arr) for (auto& i : arr)
#define sz(x) ((ll)(x).size())

int main() {
    cin.tie(0)->sync_with_stdio(0);

    ll r, c;
    cin >> r >> c;
    vi start_val(c);
    repe(v, start_val) cin >> v;

    p2 start, goal;
    cin >> start.second >> start.first >> goal.second >> goal.first;

    auto cost_at = [&](p2 p) {
        return start_val[p.second] - p.first;
    };

    vvi dist(r, vi(c, inf));
    map<p2, p2> parent;

    vector<p2> dirs = { {0,1},{0,-1},{1,0},{-1,0} };

    // Dijkstra initialization
    priority_queue<pair<ll, p2>> pq;
    dist[start.first][start.second] = cost_at(start);
    pq.emplace(-dist[start.first][start.second], start);

    while (!pq.empty()) {
        auto [d, p] = pq.top(); pq.pop();
        d = -d; // convert back to positive

        if (p == goal) break;

        if (d > dist[p.first][p.second]) continue; // skip outdated entries

        repe(dir, dirs) {
            p2 np = { p.first + dir.first, p.second + dir.second };
            if (np.first < 0 || np.second < 0 || np.first >= r || np.second >= c) continue;

            ll nd = dist[p.first][p.second] + cost_at(np);
            if (nd < dist[np.first][np.second]) {
                dist[np.first][np.second] = nd;
                parent[np] = p;
                pq.emplace(-nd, np);
            }
        }
    }

    cout << dist[goal.first][goal.second] << "\n";

    // Reconstruct path
    set<p2> path;
    p2 cur = goal;
    while (cur != start) {
        path.insert(cur);
        cur = parent[cur];
    }
    path.insert(start);

    // Print grid with path marked
    rep(i, min(10LL,r)) {
        rep(j, c) {
            if ((r-1-i)==0&&j==0) cout << "0 ";
            else if (path.count({ r-1-i, j })) cout << "* ";
            else cout << ". ";
        }
        cout << "\n";
    }

    return 0;
}
