#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
const ll INF = (ll)4e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C;
    if (!(cin >> R >> C)) return 0;

    vector<ll> start_val(C);
    for (ll &v : start_val) cin >> v;

    pii start, goal;
    cin >> start.second >> start.first >> goal.second >> goal.first;

    auto cost_at = [&](int row, int col) -> ll {
        return start_val[col] - (ll)row;
    };

    auto id = [&](int row, int col) -> int {
        return (ll)row * C + col;
    };

    ll V = (ll)R * C;
    vector<ll> dist(V, INF);
    int s = id(start.first, start.second);
    int g = id(goal.first, goal.second);

    dist[s] = cost_at(start.first, start.second);

    const int dr[4] = {0,0,1,-1};
    const int dc[4] = {1,-1,0,0};

    for (int iter = 0; iter < V - 1; ++iter) {
        bool changed = false;

        for (int row = R-1; row >= 0; row--) {
            for (int col = 0; col < C; ++col) {
                ll u = id(row, col);
                if (dist[u] == INF) continue;

                ll du = dist[u];
                for (int k = 0; k < 4; ++k) {
                    int nr = row + dr[k];
                    int nc = col + dc[k];
                    if (nr < 0 || nc < 0 || nr >= R || nc >= C) continue;
                    int v = id(nr, nc);
                    ll nd = du + cost_at(nr, nc);
                    if (nd < dist[v]) {
                        dist[v] = nd;
                        changed = true;
                    }
                }
            }
        }
        for (int row = 0; row < R; row++) {
            for (int col = 0; col < C; ++col) {
                ll u = id(row, col);
                if (dist[u] == INF) continue;

                ll du = dist[u];
                for (int k = 0; k < 4; ++k) {
                    int nr = row + dr[k];
                    int nc = col + dc[k];
                    if (nr < 0 || nc < 0 || nr >= R || nc >= C) continue;
                    int v = id(nr, nc);
                    ll nd = du + cost_at(nr, nc);
                    if (nd < dist[v]) {
                        dist[v] = nd;
                        changed = true;
                    }
                }
            }
        }

        if (!changed) break;
    }

    cout << dist[g] << '\n';
    return 0;
}
