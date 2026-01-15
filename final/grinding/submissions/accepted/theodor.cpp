#include <bits/stdc++.h>
using namespace std;
    
    
#define INF ((ll)(1e9+7))
#define fo(i, n) for(ll i=0;i<((ll)n);i++)
#define deb(x) cout << #x << " = " << (x) << endl
#define deb2(x, y) cout << #x << " = " << (x) << ", " << #y << " = " << (y) << endl
#define pb push_back
typedef long long ll;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;
    
    
int main(){
    cin.tie(0)->sync_with_stdio(0);
    // cout << fixed << setprecision(20);
    
    
    ll n, b, m, val1, val2;
    cin >> n >> b;
    ll ans = b, best = 1, curr=1;
    ll temp = 0;
    while(n--){
        cin >> m;
        ll c1 = 0;
        ll c2 = 0;
        vl s1 = {0}, s2 = {0};
        fo(i, m){
            cin >> val1 >> val2;
            c1 = max(c1, val1-c2);
            c2+=val2;
            s1.pb(c1);
            s2.pb(c2);
        }
        while(1){
            if(curr>=b)break;
            auto idx = upper_bound(s1.begin(), s1.end(), curr);
            best = max(s2[idx-s1.begin()-1], best);
            ans = min(ans, temp+(b-curr+best-1ll)/best);
            // deb2(ans, best);
            // deb2(curr, temp);
            if(idx == s1.end()){
                curr+=c2;
                temp++;
                break;
            }
            ll left = (*idx)-curr;
            ll rounds = (left+best-1ll)/best;
            temp+=rounds;
            curr+=rounds*best;    
        }
        ans = min(ans, temp+(b-curr+b-1ll)/best);
    }
    cout << ans;


    return 0;
}