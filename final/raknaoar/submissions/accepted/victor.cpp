// #pragma GCC target ("avx,avx2,fma")
// #pragma GCC optimize ("Ofast,inline") // O1 - O2 - O3 - Os - Ofast
// #pragma GCC optimize ("unroll-loops")
#include <bits/stdc++.h>

using namespace std;

#define rep(i, a, b) for (ll i = (a); i < (b); ++i)
#define per(i, a, b) for (ll i = (b) - 1; i >= (a); --i)
#define trav(a, x) for (auto &a : x)

#define all(x) x.begin(), x.end()
#define sz(x) (ll)x.size()
#define pb push_back
#define debug(x) cout<<#x<<" = "<<x<<endl

#define umap unordered_map
#define uset unordered_set

typedef pair<int, int> ii;
typedef pair<int, ii> iii;
typedef vector<int> vi;
typedef vector<ii> vii;
typedef vector<vi> vvi;

typedef long long ll;
typedef pair<ll,ll> pll;
typedef pair<ll,pll> ppll;
typedef vector<ll> vll;
typedef vector<pll> vpll;
typedef vector<vll> vvll;

const ll INF = 1'000'000'007;

ll n,q;
map<ll,ll> ans;
vpll islands;

int main() {
    cin.tie(0)->sync_with_stdio(0);
    cin.exceptions(cin.failbit);

    cin>>n>>q;
    rep(i,0,n) {
        ll k;
        cin>>k;
        rep(j,0,k) {
            ll s;
            cin>>s;
            islands.emplace_back(s,i);
        }
    }

    vll counts(n,0);
    sort(all(islands),greater<>());
    ll mxcnt=0,id=n-1;
    ans[INF]=id;

    trav(island,islands) {
        ll s,i;
        tie(s,i)=island;

        ++counts[i];
        if(pll(counts[i],i)>pll(mxcnt,id)) {
            mxcnt=counts[i];
            id=i;
        }

        ans[s]=id;
    }

    while(q--) {
        ll s;
        cin>>s;
        cout<<ans.lower_bound(s)->second+1<<endl;
    }
    return 0;
}