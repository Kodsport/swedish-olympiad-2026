#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for (int i = a; i < (b); ++i)
#define trav(a, x) for (auto &a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef long double ld;
typedef unsigned long long ull;

ll n;
vl W;
vl small_divs;
ll totw = 0;

bool test(ll parts){
    vi pek(n, -1);
    int p = 0;
    ll sum = totw / parts;
    ll cur = 0;
    rep(c1,0,n){
        cur += W[c1];
        if(cur >= sum){
            p = (c1+1)%n;
            break;
        }
    }

    rep(c1,0,n){
        if(cur == sum){
            pek[c1] = p;
        }
        cur -= W[c1];
        while(cur < sum){
            cur += W[p];
            p = (p+1)%n;
        }

       // cerr << parts << ":   " << pek[c1]+1 << "\n";
    }

    rep(c1,0,n){
        if(pek[c1] != -1){
            int i = c1;
            while(pek[i] != -1){
                int j = pek[i];
                pek[i] = -1;
                i = j;
            }
            if(i == c1)return 1;
        }
    }

    return 0;

}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    ll a,b,c,d;

    cin >> n;
    rep(c1,0,n){
        cin >> a;
        W.push_back(a);
        totw += a;
    }

    for(ll d = 2; d <= n; d++){
        if(totw%d == 0)small_divs.push_back(d);
    }

    reverse(all(small_divs));

    trav(p, small_divs){
        if(test(p)){
            cout << n-p << "\n";
            return 0;
        }
    }
    cout << n-1 <<"\n";


    return 0;
}