#include <bits/stdc++.h>
using namespace std;
     
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;

ll n,ca,cb;
string s;
ll tot = 0;
set<pll> H;
ll extra = 0;

ll f(){
    ll valley = (*H.begin()).first-extra;
    return abs(valley) + abs(tot-valley);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
     
    ll a,b,c,d;
    
    cin >> n >> ca >> cb;
    cin >> s;

    rep(c1,0,n){
        H.insert({tot,c1});
        if(s[c1] == '('){
            tot++;
        }
        else{
            tot--;
        }
    }

    H.insert({tot,n});

    ll ans = ca*f();
    rep(c1,0,n){
        H.erase({extra,c1});
        if(s[c1] == '('){
            extra++;
        }
        else{
            extra--;
        }
        ans = min(ans, ll(c1+1)*cb + ca*f());
    }

    cout << ans << "\n";
 
    return 0;
}