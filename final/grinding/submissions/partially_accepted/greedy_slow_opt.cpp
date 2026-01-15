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

    auto compute_breakpoint = [&](int dungeon_index)
    {
        ll start = 1e10 + 1;
        ll lo = -1;
        ll hi = start;

        while (lo + 1 < hi)
        {
            ll mid = (lo + hi) / 2;
            if (simulate_dungeon(dungeon_index, mid).first >= best_prev) hi = mid;
            else lo = mid;
        }
        return hi == start ? -inf : hi;
    };

    rep(i, n) // i just unlocked dungeon i
    {
        auto cost_to_win = [&]()
        {
            ll str = strength;
            ll newruns = 0;

            if (best_prev)
            {
                ll breakeven_strength = compute_breakpoint(i);
                if (breakeven_strength == -inf) breakeven_strength = b;
                ll num_prev = (breakeven_strength - strength + best_prev - 1) / best_prev;
                if (num_prev > 0)
                {
                    str += num_prev * best_prev;
                    newruns += num_prev;
                }
            }

            while (str < b)
            {
                newruns++;
                str += simulate_dungeon(i, str).first;
            }
            return newruns;
        };
        ans = min(ans, runs + cost_to_win());

        if (best_prev)
        {
            ll breakeven_strength = compute_breakpoint(i);
            if (breakeven_strength == -inf) breakeven_strength = lo_strength[i];
            ll num_prev = (breakeven_strength - strength + best_prev - 1) / best_prev;
            if (num_prev > 0)
            {
                strength += num_prev * best_prev;
                runs += num_prev;
            }
        }

        while (strength < lo_strength[i])
        {
            runs++;
            strength += simulate_dungeon(i, strength).first;
        }
        runs++;
        ll dungeon_profit = simulate_dungeon(i, strength).first;
        best_prev = max(best_prev, dungeon_profit);
        strength += dungeon_profit;
    }
    cout << ans << '\n';

    return 0;
}
