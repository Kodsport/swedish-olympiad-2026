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
        int i = 1;
        while (i<sz(arr))
        {
            if (arr[i] > arr[i - 1]) i++;
            else
            {
                int v = arr[i];
                arr.erase(begin(arr) + i);
                training_camp.push_back(v);
            }
        }
        repe(v, arr) cout << v << ' ';
        cout << '\n';
        arr = training_camp;
    }
    repe(v, arr) cout << v << ' ';
    cout << '\n';

    return 0;
}
