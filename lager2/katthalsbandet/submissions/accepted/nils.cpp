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

ll solve(ll n, ll ca, ll cb, string &s){
    ll tot = 0;
    set<pll> H;
    ll extra = 0;
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

    ll valley = (*H.begin()).first;
    ll ans = ca*(abs(valley) + abs(tot-valley));

    set<pll> SUFF;
    ll plus = 0;
    ll minus = 0;

    rep(c1,0,n){
        H.erase({extra,c1});
        SUFF.insert({extra, c1});
        if(s[c1] == '('){
            extra++;
            plus++;
        }
        else{
            extra--;
            minus++;
        }

        ll valley = (*H.begin()).first-extra;
        ll start_suff = tot-extra-valley;
        ll valley_suff = min(0ll,(*SUFF.begin()).first+start_suff);
        ll cc = ca-cb;

        ll rem_suff = plus;
        ll rest = abs(tot-valley-valley_suff);

        ll rest_cost = min(rest,rem_suff)*cc;
        rest -= min(rest,rem_suff);
        rest_cost += rest*ca;

        ll temp = ll(c1+1)*cb + ca*abs(valley) + cc*abs(valley_suff) + rest_cost;
        ans = min(ans, temp);

    }
        return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
     
    ll a,b,c,d;
    
    ll n,ca,cb;
    string s;
    cin >> n >> ca >> cb;
    cin >> s;

    ll ans = solve(n,ca,cb,s);
    cout << ans << "\n";
    
 
    return 0;
}