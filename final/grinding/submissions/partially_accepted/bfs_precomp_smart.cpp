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

    int k, b;
    cin >> k >> b;
    vector<vector<p2>> dungeons(k);
    rep(i, k)
    {
        int n;
        cin >> n;
        dungeons[i].resize(n);
        repe(x, dungeons[i]) cin >> x.first >> x.second;
    }

    auto simulate_dungeon = [&](int dungeon_index, ll strength)
    {
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
        return make_pair(strength, wins);
    };

    vector<vector<p2>> precomp(k, vector<p2>(b + 1));
    rep(i, k)
    {
        rep(j, b + 1) precomp[i][j] = simulate_dungeon(i, j);
    }
    vvi precomp_prefmax(b + 1, vi(k));
    rep(i, b + 1)
    {
        precomp_prefmax[i][0] = precomp[0][i].first;
        repp(j, 1, k) precomp_prefmax[i][j] = max(precomp_prefmax[i][j - 1], precomp[j][i].first);
    }

    vvi vis(k + 1, vi(b + 1));
    queue<tuple<int, int, ll>> q;
    q.emplace(1, 0, 1);
    while (sz(q))
    {
        auto [unlocked, runs, strength] = q.front();
        q.pop();
        if (strength >= b)
        {
            cout << runs << "\n";
            return 0;
        }
        if (vis[unlocked][strength]) continue;
        vis[unlocked][strength] = 1;

        ll best_strength = precomp_prefmax[strength][unlocked - 1];

        q.emplace(unlocked, runs + 1, best_strength);
        if (unlocked < k)
        {
            auto [new_strength, wins] = precomp[unlocked - 1][strength];
            if (wins) q.emplace(unlocked + 1, runs + 1, new_strength);
        }
    }
    assert(0);

    return 0;
}
