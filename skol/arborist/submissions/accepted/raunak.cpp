#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#define dbg(x) cout << #x << " = " << x << endl

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(cin.failbit);

	ll n, k;
	cin >> n >> k;
	
	vector<pair<ll, ll>> trees(n);
	rep(i, 0, n) cin >> trees[i].first;
	rep(i, 0, n) cin >> trees[i].second;
	sort(all(trees));

	vector<ll> sum(1<<n);

	rep(b, 0, 1<<n) {
		ll cur = 0;
		rep(i, 0, n) {
			if (b & (1<<i)) {
				cur += trees[i].second;
			}
		}
		sum[b] = cur;
	}

	vector<ll> dp(1<<n, 1e16);
	dp[0] = 0;

	rep(mask, 0, 1<<n) {
		ll rem = ((1<<n) - 1) ^ mask;
		for(ll submask = rem; submask; submask = (submask - 1) & rem) {
			if (sum[submask] <= k) {
				ll plants = __builtin_popcountll(submask);
				ll mx = 0;
				rep(i, 0, n) if (submask & (1<<i)) mx = i;
				ll dist = 2 * trees[mx].first; 
				dp[mask | submask] = min(dp[mask] + dist + 2 * plants, dp[mask | submask]);
			}
		}
	}

	cout << dp[(1<<n) - 1];
}


