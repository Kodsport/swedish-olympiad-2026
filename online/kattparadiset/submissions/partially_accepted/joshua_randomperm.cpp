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

    auto start = chrono::high_resolution_clock::now();
    int n, k;
    cin >> n >> k;

    vector<string> colors;
    vi perm;
    rep(i, k)
    {
        string color;
        int cnt;
        cin >> color >> cnt;
        colors.push_back(color);
        rep(j, cnt) perm.push_back(i);
    }

    auto check = [&]()
    {
        if (perm[0] == perm.back()) return false;
#pragma GCC ivdep
        rep(i, n - 1)
        {
            if (perm[i] == perm[i + 1]) return false;
        }
        return true;
    };

    mt19937 rng(42);
    int its = 0;
    while (true)
    {
        if (its++ % 1000==0)
        {
            if (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now()-start).count() > 950)
            {
                break;
            }
        }
        if (check())
        {
            cout << "Ja\n";
            repe(p, perm)
            {
                cout << colors[p] << " ";
            }
            return 0;
        }
        shuffle(all(perm), rng);
    }
    cout << "Nej\n";
	
    return 0;
}
