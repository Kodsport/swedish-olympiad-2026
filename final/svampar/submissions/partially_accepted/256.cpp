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
    vi curr(n);
    while (*max_element(all(curr)) < *max_element(all(targets)))
    {
        vector<Action> round(n);
        rep(i, n)
        {
            if (curr[i] < targets[i]) round[i] = { '+', i }, curr[i]++;
            else round[i] = { 'p', -1 };
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
