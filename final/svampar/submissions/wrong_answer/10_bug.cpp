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

struct Action
{
    char kind;
    ll j;
    void print() {
        if (kind == 'p') cout << kind << '\n';
        else cout << kind << ' ' << j + 1 << '\n';
    }
    Action() : kind('p'), j(-1) {}
    Action(char c, ll i) : kind(c), j(i) {}
    Action(char c, int i) : kind(c), j(i) {}
};

int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vi targets(n);
    repe(t, targets) cin >> t;

    vector<vector<Action>> rounds;
    {
        vector<Action> round(n);
        round[256] = { '+', 256 };
        rounds.push_back(round);
    }

    rep(b, 8)
    {
        vector<Action> round(n);
        rep(i, 256)
        {
            if (i & (1 << b))
            {
                // cout << "OGEY" << '\n';
                round[i] = { '^', 256 };
            }
        }

        round[256] = { '<', 256 };
        rounds.push_back(round);
    }

    // dooes not reset the 256 (it can't be cancelled)
    // {
    //     vector<Action> round(n);
    //     round[256] = { '^', 256 };
    //     rounds.push_back(round);
    // }

    vi vals(n);
    rep(i, 256) vals[i] = i;
    vals[256] = 0;

    {
        vector<Action> round(n);
        rep(i, n)
        {
            int what = targets[i] ^ vals[i];
            round[i] = { '^', what };
        }
        rounds.push_back(round);
    }

    cout << sz(rounds) << '\n';
    repe(round, rounds)
    {
        repe(v, round) v.print();
    }

    return 0;
}
