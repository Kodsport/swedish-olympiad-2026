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

    auto sorted = [&](vi& arr)
    {
        repp(i, 1, sz(arr)) if (arr[i] <= arr[i - 1]) return false;
        return true;
    };

    while (!sorted(arr) && sz(arr))
    {
        vi training_camp = {  };
        vi new_arr = { arr[0] };
        int i = 1;
        repp(i, 1, sz(arr))
        {
            if (arr[i] > new_arr.back()) new_arr.push_back(arr[i]);
            else training_camp.push_back(arr[i]);
        }
        repe(v, new_arr) cout << v << ' ';
        cout << '\n';
        arr = training_camp;
    }
    repe(v, arr) cout << v << ' ';
    cout << '\n';

    return 0;
}
