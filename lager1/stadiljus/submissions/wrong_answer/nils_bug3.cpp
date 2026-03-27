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
const int MAXN = 300001;

template<class T>
struct RMQ {
	vector<vector<T>> jmp;
	RMQ(const vector<T>& V) : jmp(1, V) {
		for (int pw = 1, k = 1; pw * 2 <= sz(V); pw *= 2, ++k) {
			jmp.emplace_back(sz(V) - pw * 2 + 1);
			rep(j,0,sz(jmp[k]))
				jmp[k][j] = max(jmp[k - 1][j], jmp[k - 1][j + pw]);
		}
	}
	T query(int a, int b) {
		assert(a < b); // or return inf if a == b
		int dep = 31 - __builtin_clz(b - a);
		return max(jmp[dep][a], jmp[dep][b - (1 << dep)]);
	}
};

vi P, PI;

vi L,R;

int build_tree(int lo, int hi, RMQ<int> &rmq){
    if(lo >= hi)return -1;
    int ni = rmq.query(lo, hi);
    int i = PI[ni];
    L[ni] = build_tree(lo,i,rmq);
    R[ni] = build_tree(i+1,hi,rmq);
    return ni;
}

int DP[MAXN][3][3] = {0};
bool DPC[MAXN][3][3] = {0};

int dp(int i, int lf, int rf){
    if(i == -1){
        if(rf == 1 || lf == 1)return nils;
        return 0;
    }
    if(DPC[i][lf][rf])return DP[i][lf][rf];

    int ans = 1 + dp(L[i], lf-lf%2, 2) + dp(R[i], 2, rf-rf%2);
    if(lf == 2 || rf == 2 || 1){
        // we dont need to be covered
        ans = min(ans, dp(L[i], lf, 0) + dp(R[i], 0, rf));
    }
    else{
        ans = min(ans, dp(L[i], lf, 1) + dp(R[i], 0, rf));
        ans = min(ans, dp(L[i], lf, 0) + dp(R[i], 1, rf));
    }
    DPC[i][lf][rf] = 1;
    DP[i][lf][rf] = ans;
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
     
    ll a,b,c,d;
 
    cin >> n;
    rep(c1,0,n){
        cin >> a;
        P.push_back(a-1);
        L.push_back(-1);
        R.push_back(-1);
        PI.push_back(-1);
    }
    rep(c1,0,n){
        PI[P[c1]] = c1;
    }

    RMQ rmq(P);

    build_tree(0,n,rmq);

    int ans = dp(n-1,0,0);
    cout << ans << "\n";



    return 0;
}