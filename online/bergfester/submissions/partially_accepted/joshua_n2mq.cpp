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

    int n, m, q;
    cin >> n >> m >> q;

    vi par(n);
    rep(i, n) cin >> par[i],par[i]--;
    int root = -1;
    rep(i, n) if (par[i] == i) root = i;
    assert(root != -1);

    vi who_lives(n, -1);
    vi which_house(m);
    rep(i, m)
    {
        int h;
        cin >> h;
        h--;
        which_house[i] = h;
        who_lives[h] = i;
    }


    auto swap_houses = [&](int a, int b)
    {
        swap(who_lives[a], who_lives[b]);
        if (who_lives[a] != -1) which_house[who_lives[a]] = a;
        if (who_lives[b] != -1) which_house[who_lives[b]] = b;
    };

    auto close_enough = [&](int u, int t, int r)
    {
        if (u == t) return true;
        rep(i, r)
        {
            u = par[u];
            if (u == t) return true;
        }
        return false;
    };

    auto mod = [&](int ind)
    {
        ind %= m;
        if (ind < 0) ind += m;
        return ind;
    };

    while (q--)
    {
        int t;
        cin >> t;

        if (t==1)
        {
            int a, b;
            cin >> a >> b;
            a--; b--;
            swap_houses(a, b);
        }
        else
        {
            int p, r;
            cin >> p >> r;
            ll best = 0;
            p--;
            int friend_ind = p;

            rep(h, n)
            {
                int ind = mod(friend_ind);
                rep(steps, m)
                {
                    if (!close_enough(which_house[ind], h, r)) break;

                    best = max(best, steps + 1);
                    ind = mod(ind + 1);
                }

                ind = mod(friend_ind);
                rep(steps, m)
                {
                    if (!close_enough(which_house[ind], h, r)) break;

                    best = max(best, steps + 1);
                    ind = mod(ind - 1);
                }
            }

            cout << best << '\n';
        }
    }

    return 0;
}
