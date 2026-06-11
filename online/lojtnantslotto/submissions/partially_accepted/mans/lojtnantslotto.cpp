
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    
    ll maxl = 5;
    ll N, T;
    cin >> N >> T;
    vector<pair<ll, ll>> castle(N);
    for (int i = 0; i < N; i++)
        cin >> castle[i].first >> castle[i].second;
    
    vector<vector<ll>> boxed(maxl+1);
    for (auto [l, g] : castle)
        boxed[l].push_back(g);
    for (int i = 0; i < boxed.size(); i++)
        sort(boxed[i].begin(), boxed[i].end());
    
    vector<ll> best(maxl*N+1, -1);
    best[0] = 0;
    vector<vector<ll>> state(maxl*N+1, vector<ll>(maxl+1, 0));
    for (int i = 1; i < best.size(); i++) {
        ll res_j = -1;
        for (int j = 1; j <= maxl; j++) {
            if (j > i)
                break;
            if (best[i-j] == -1)
                continue;
            int p = state[i-j][j];
            if (p < boxed[j].size()) {
                if (res_j == -1)
                    res_j = j;
                else if (best[i-j]+boxed[j][p] < best[i-res_j]+boxed[res_j][state[i-res_j][res_j]]) {
                    res_j = j;
                }
            }
        }
        if (res_j != -1) {
            best[i] = best[i-res_j]+boxed[res_j][state[i-res_j][res_j]];
            for (int j = 1; j <= maxl; j++)
                state[i][j] = state[i-res_j][j];
            state[i][res_j]++;
        }
    }

    ll ans = 0;
    for (int i = 0; i < best.size(); i++) {
        if (best[i] <= T && best[i] != -1)
            ans = i;
    }
    cout << ans << endl;
    exit(0);
    
}
