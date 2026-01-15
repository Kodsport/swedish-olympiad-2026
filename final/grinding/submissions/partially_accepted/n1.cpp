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

    ll ans = inf;
    ll strength = 1;

    vector<p2>& dungeon = dungeons[0];
    vi breakpoints = { dungeon[0].first };
    vi breakpoint_gain = { dungeon[0].second };
    {
        ll start_strength = dungeon[0].first;
        ll extra_strength = dungeon[0].second;
        repp(j, 1, sz(dungeon))
        {
            if (start_strength + extra_strength >= dungeon[j].first)
            {
                //
            }
            else
            {
                ll delta = dungeon[j].first - (start_strength + extra_strength);
                start_strength += delta;
            }
            breakpoints.push_back(start_strength);
            extra_strength += dungeon[j].second;
            breakpoint_gain.push_back(extra_strength);
        }
        breakpoints.push_back(inf);
        breakpoint_gain.push_back(extra_strength);
    }

    auto min_to_get = [&](ll target)
    {
        ll str = strength;
        ll newruns = 0;

        while (str < target)
        {
            assert(str >= breakpoints[0]);
            auto it = upper_bound(all(breakpoints), str);
            assert(it != begin(breakpoints));
            it = prev(it); // last with breakpoint <= str
            ll curr_gain = breakpoint_gain[it - begin(breakpoints)];
            ll next_breakpoint;
            if (*next(it) == inf) // no improvement possible
            {
                next_breakpoint = target;
            }
            else next_breakpoint = min(target, *next(it));
            ll num_runs = (next_breakpoint - str + curr_gain - 1) / curr_gain;
            str += num_runs * curr_gain;
            newruns += num_runs;
        }
        return make_pair(str, newruns);
    };

    cout << min_to_get(b).second;

    return 0;
}
