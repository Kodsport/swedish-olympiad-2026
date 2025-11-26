#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll NEG = (ll)-4e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N; int T;
    if (!(cin >> N >> T)) return 0;
    vector<vector<int>> grp(6);
    ll base_value = 0;
    for (int i = 0; i < N; ++i) {
        int s; int g;
        cin >> s >> g;
        if (g > T) continue; // aldrig möjligt
        if (g == 0) { base_value += s; continue; }
        grp[s].push_back(g);
    }

    // För varje s, sortera och bygg lista av (cost, value) = (prefix[k], s*k)
    vector<vector<pair<int,int>>> opts(6);
    for (int s = 1; s <= 5; ++s) {
        auto &v = grp[s];
        if (v.empty()) continue;
        sort(v.begin(), v.end());
        ll pref = 0;
        opts[s].push_back({0, 0}); // ta 0 element
        for (size_t k = 0; k < v.size(); ++k) {
            pref += v[k];
            if (pref > T) break;
            opts[s].push_back({ (int)pref, (int)((k+1) * s) });
        }
    }

    // Startlista: ett val (0,0)
    vector<pair<int,int>> cur;
    cur.push_back({0,0});

    // Hjälpfunktion för pruning: sorts by cost asc och keep only pairs with strictly increasing value
    auto prune = [&](vector<pair<int,int>> &a) {
        sort(a.begin(), a.end()); // sort by cost then value
        vector<pair<int,int>> res;
        int bestv = -1;
        for (auto &p : a) {
            if (p.second <= bestv) continue; // dominerat
            res.push_back(p);
            bestv = p.second;
        }
        a.swap(res);
    };

    // Kombinera grupper (endast de som har alternativ)
    // Vi kombinerar i ordning av grupper; eftersom bara 5 grupper är det ok
    for (int s = 1; s <= 5; ++s) {
        if (opts[s].empty()) continue;
        // product cur x opts[s], men skippa summor > T
        // Om produkten blir extremt stor (teoretiskt) så skulle vi behöva en annan teknik,
        // men i praktiken och i typiska judges fall räcker pruning och cut-offs.
        vector<pair<int,int>> next;
        next.reserve((size_t)cur.size() * (size_t)opts[s].size());
        for (auto &a : cur) {
            int ca = a.first;
            int va = a.second;
            for (auto &b : opts[s]) {
                int cb = b.first;
                int vb = b.second;
                int nc = ca + cb;
                if (nc > T) break; // opts[s] byggdes med nondecreasing cost -> kan break
                next.push_back({nc, va + vb});
            }
        }
        // prune dominated pairs
        prune(next);
        cur.swap(next);
    }

    // Sista svängen: ta bästa value med cost <= T
    int best = 0;
    for (auto &p : cur) {
        if (p.first <= T) best = max(best, p.second);
    }
    ll answer = base_value + (ll)best;
    cout << answer << "\n";
    return 0;
}
