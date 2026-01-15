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
        else cout << kind << ' ' << j+1 << '\n';
    }
};

int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vi targets(n);
    repe(t, targets) cin >> t;

    vector<vector<Action>> rounds;
    while (*max_element(all(targets)) > 0)
    {
        vector<Action> round(n);
        rep(i, n)
        {
            if (targets[i]%2) round[i] = { '+', i }, targets[i]-=1;
            else round[i] = { '<', i }, targets[i]/=2;
        }
        rounds.push_back(round);
    }
    reverse(all(rounds));

    cout << sz(rounds) << '\n';
    repe(round, rounds)
    {
        repe(v, round) v.print();
    }

    return 0;
}
