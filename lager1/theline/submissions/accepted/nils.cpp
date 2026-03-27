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

ll n,k;
     
const ll nils = 1000000007;
const int MAXK = 10001;

int LO[MAXK];
int HI[MAXK];
ll L[MAXK];
ll R[MAXK];

vl L_sorted;

ll bonus = 0;
ll tot = 0;

ll CS1[MAXK] = {0};
ll CS2[MAXK] = {0};

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
     
    ll a,b,c,d;
 
    vector<pll> RL;
    cin >> n >> k;
    rep(c1,0,k){
        cin >> a >> b;
        if(a > b)swap(a,b);
        RL.push_back({b, a});
        L_sorted.push_back(a);
    }

    sort(all(L_sorted)); // dont sort to get slower (O(K^3)?) solution
    sort(all(RL));
    
    rep(c1,0,k){
        R[c1] = RL[c1].first;
        L[c1] = RL[c1].second;
        LO[c1] = c1;
        HI[c1] = c1;
        tot += R[c1]-L[c1];
    }

    rep(c1,0,k){
        rep(c2,0,k+1){
            CS1[c2] = 0;
            CS2[c2] = 0;
        }
        rep(c2,0,k){
            c = R[c2]-L[c2]-1-abs(L[c2] - L_sorted[c1]);
            //cerr << c << " ";
            if(c > 0){
                while(LO[c2] > 0 && R[c2]-R[LO[c2]-1] <= c){
                    LO[c2]--;
                }
                while(R[c2]-R[LO[c2]] > c){
                    LO[c2]++;
                }
                while(R[HI[c2]]-R[c2] > c){
                    HI[c2]--;
                }
                while(HI[c2] < k-1 && R[HI[c2]+1]-R[c2] <= c){
                    HI[c2]++;
                }

                CS1[LO[c2]] += c - (R[c2]-R[LO[c2]]);
                CS1[HI[c2]+1] -= c - (R[HI[c2]] - R[c2]);

                CS2[LO[c2]]++;
                CS2[c2]-=2;
                CS2[HI[c2]]++;
                //cerr << c << "  -  "<< LO[c2] << " " << HI[c2] << "  lohi\n";
            }
        }//cerr << "           --      cc\n";
        
        ll deri = 0;
        ll cs = 0;
        rep(c2,0,k){
            if(c2 > 0){
                CS1[c2] += (R[c2]-R[c2-1])*deri;
            }
            cs += CS1[c2];
            bonus = max(bonus, cs);
            deri += CS2[c2];
        }    
    }


    cout << tot-bonus << "\n";


    return 0;
}