#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int,int>;
const ll INF = (ll)4e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll r, c;
    if (!(cin >> r >> c)) return 0;
    vector<ll> start_val(c);
    for (ll &v : start_val) cin >> v;

    // preserve the exact input ordering from original code:
    // they read "col row" pairs into (second, first)
    pii start, goal; // (row, col)
    cin >> start.second >> start.first >> goal.second >> goal.first;

    auto cost_at = [&](int row, int col) -> ll {
        return start_val[col] - (ll)row;
    };

    vector<vector<ll>> dist((size_t)r, vector<ll>((size_t)c, INF));
    // min-heap: (distance, (row,col))
    using Item = pair<ll, pair<int,int>>;
    priority_queue<Item, vector<Item>, greater<Item>> pq;

    dist[start.first][start.second] = cost_at(start.first, start.second);
    pq.push({dist[start.first][start.second], {start.first, start.second}});

    const int dr[4] = {0,0,1,-1};
    const int dc[4] = {1,-1,0,0};

    while (!pq.empty()) {
        auto [d, rc] = pq.top(); pq.pop();
        int row = rc.first, col = rc.second;
        if (d != dist[row][col]) continue; // stale entry
        if (row == goal.first && col == goal.second) {
            cout << d << '\n';
            return 0;
        }
        for (int k = 0; k < 4; ++k) {
            int nr = row + dr[k];
            int nc = col + dc[k];
            if (nr < 0 || nc < 0 || nr >= (int)r || nc >= (int)c) continue;
            ll nd = d + cost_at(nr, nc);
            if (nd < dist[nr][nc]) {
                dist[nr][nc] = nd;
                pq.push({nd, {nr, nc}});
            }
        }
    }

    // preserve original behavior on unreachable: original asserted(0)
    assert(false);
    return 0;
}
