#pragma GCC optimize("O3")
#include <bits/allocator.h>
#pragma GCC target("avx2")
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

    int k,b;
    cin >> k >> b;
    vector<vector<p2>> dungeons(k);
    rep(i,k)
    {
        int n;
        cin >> n;
        dungeons[i].resize(n);
        repe(x, dungeons[i]) cin >> x.first >> x.second;
    }

    auto simulate_dungeon = [&](int dungeon_index, int strength)
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
        return make_pair(strength,wins);
    };

    vector<vector<short>> precomp(b+1, vector<short>(k));
    vector<vector<char>> precomp_pass(b+1, vector<char>(k));
    rep(i,b+1)
    {
        rep(j,k) tie(precomp[i][j], precomp_pass[i][j]) = simulate_dungeon(j, i);
    }

    vvi vis(k+1, vi(b+1));
    queue<tuple<int,int,ll>> q;
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

        short best_strength = strength;
        rep(i, unlocked) best_strength = max(best_strength, precomp[strength][i]);
        
        q.emplace(unlocked, runs + 1, best_strength);
        if (unlocked<k)
        {
            if (precomp_pass[strength][unlocked-1]) q.emplace(unlocked+1,runs+1, precomp[strength][unlocked-1]);
        }
    }
    assert(0);
    
    return 0;
}
