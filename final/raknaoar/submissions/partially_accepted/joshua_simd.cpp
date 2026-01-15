// O(TQ)
#pragma GCC otimize("O3")
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

const int maxt = 5e5+10;
int main()
{
    cin.tie(0)->sync_with_stdio(0);

    int n, q;
    cin >> n >> q;

    vector<int> islands(maxt);
    vector<int> mystart(n+1);
    int it = 0;
    rep(i, n)
    {
        mystart[i] = it;
        int k;
        cin >> k;
        rep(j, k) cin >> islands[it++];
    }
    mystart.back() = it;

    while (q--)
    {
        int x;
        cin >> x;
        
        pair<int,int> best = p2(0, n);

        for (int i = 0; i < n; i++)
        {
            int amount = 0;
            #pragma GCC ivdep
            repp(j, mystart[i], mystart[i+1]) amount += islands[j] >= x;
            best = max(best, make_pair(amount, i + 1));
        }

        cout << best.second << '\n';
    }

    return 0;
}
