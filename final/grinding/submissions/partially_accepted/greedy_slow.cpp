#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<ll>;
using vvi = vector<vi>;
using p2 = pair<ll, ll>;
const ll inf = 1e18;

#define rep(i,n) for (ll i = 0; i < (n); i++)
#define repp(i,a,n) for (ll i = (a); i < (n); i++)
#define repe(i, arr) for (auto& i : arr)
#define all(x) begin(x),end(x)
#define sz(x) ((ll)(x).size())


int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int n, b;
    cin >> n >> b;

    vector<vector<p2>> dungeons(n);
    rep(i, n)
    {
        int m;
        cin >> m;
        dungeons[i].resize(m);
        rep(j, m) cin >> dungeons[i][j].first >> dungeons[i][j].second;
    }

    auto simulate_dungeon = [&](int dungeon_index, ll strength)
    {
        ll start_strength = strength;
        bool wins = true;
        for (auto [req, reward] : dungeons[dungeon_index])
        {
            if (strength >= req)
            {
                strength += reward;
            }
            else
            {
                wins = false;
                break;
            }
        }
        return make_pair(strength - start_strength, wins);
    };

    vi lo_strength(n);

    rep(i, n)
    {
        ll lo = -1;
        ll hi = 1e9 + 1;
        while (lo + 1 < hi)
        {
            ll mid = (lo + hi) / 2;
            if (simulate_dungeon(i, mid).second)
            {
                hi = mid;
            }
            else lo = mid;
        }
        lo_strength[i] = hi;
    }

    ll ans = inf;
    ll runs = 0;
    ll strength = 1;
    ll best_prev = 0;
    rep(i, n) // i just unlocked dungeon i
    {
        auto cost_to_win = [&]()
        {
            ll str = strength;
            ll newruns = 0;

            while (str < b)
            {
                newruns++;
                str += max(best_prev, simulate_dungeon(i, str).first);
            }

            return newruns;
        };
        ans = min(ans, runs + cost_to_win());

        while (strength < lo_strength[i])
        {
            runs++;
            strength += max(best_prev, simulate_dungeon(i, strength).first);
        }
        runs++;
        p2 res = simulate_dungeon(i, strength);
        best_prev = max(best_prev, res.first);
        strength += res.first;
    }
    cout << ans << '\n';

    return 0;
}
