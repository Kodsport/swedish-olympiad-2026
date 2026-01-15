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


void solve(int n, vi& targets, int hibit) // solves it in hibit + 1 rounds
{
    int sentinel = 1 << hibit;
    vector<vector<Action>> rounds;
    {
        vector<Action> round(n);
        round[sentinel] = { '+', sentinel };
        rounds.push_back(round);
    }

    rep(b, hibit)
    {
        vector<Action> round(n);
        rep(i, sentinel)
        {
            if (i & (1 << b))
            {
                // cout << "OGEY" << '\n';
                round[i] = { '^', sentinel };
            }
        }

        if (b!=hibit-1) round[sentinel] = { '<', sentinel };
        rounds.push_back(round);
    }

    vi vals(n);
    rep(i, sentinel) vals[i] = i;
    vals[sentinel] = sentinel/2;

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
}

int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vi targets(n);
    repe(t, targets) cin >> t;
    int num_big = 0;
    repe(t, targets) num_big += t >= 128;
    if (num_big == 0) solve(n, targets, 7);
    else // build one 128. the rest build their low bits. COLLIDE!
    {
        int a=-1,b=-1;
        rep(i,n) rep(j,n)
        {
            if (i==j) continue;
            if (targets[i] == targets[j]) {
                a=i;
                b=j;
            }
        }
        // pidgeonhole principle. N=512, maxval=255
        assert(a!=-1);

        // a becomes 128 in 8 rounds
        // b becomes lower bits of a in 8 rounds
        vector<vector<Action>> rounds;
        {
            vector<Action> round(n);
            round[a] = { '+', a };
            rounds.push_back(round);
        }

        rep(b, 7)
        {
            vector<Action> round(n);
            rep(i, n)
            {
                if (targets[i] & (1<<b))
                {
                    round[i] = { '^', a };
                }
            }

            if (b!=6) round[a] = { '<', a };
            rounds.push_back(round);
        }


        {
            vector<Action> round(n);
            round[a] = {'^', b};
            round[b] = {'^', a};
            rep(i,n)
            {
                if (i==a || i==b) continue;
                if (targets[i]>=128)
                {
                    round[i] = {'^', a};
                }
            }
            rounds.push_back(round);
        }

        cout << sz(rounds) << '\n';
        repe(round, rounds)
        {
            repe(v, round) v.print();
        }
    }

    return 0;
}
