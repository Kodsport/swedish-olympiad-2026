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

    int n;
    cin >> n;
    vi arr(n);
    repe(v, arr) cin >> v;

    vi deleted(n);
    vector<queue<int>> occs(11);
    rep(i, n) occs[arr[i]].push(i);
    rep(i, n)
    {
        if (deleted[i]) continue;
        vi fragment = { arr[i] };
        occs[arr[i]].pop();
        while (true)
        {
            int smolest = 1e9;
            int smolind = -1;
            repp(val, fragment.back()+1, 11)
            {
                queue<int>& q = occs[val];
                while (sz(q) && deleted[q.front()]) q.pop();
                if (sz(q) && q.front() < smolest)
                {
                    smolest = q.front();
                    smolind = val;
                }
            }
            if (smolind != -1)
            {
                fragment.push_back(smolind);
                deleted[occs[smolind].front()] = 1;
                occs[smolind].pop();
            }
            else break;
        }

        repe(v, fragment) cout << v << ' ';
        cout << '\n';
    }

    cout << '\n';

    return 0;
}
