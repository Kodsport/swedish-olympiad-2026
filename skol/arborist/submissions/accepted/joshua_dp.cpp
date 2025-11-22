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

const int maxn = 16;
ll dp[1 << maxn];

ll n, K;
vi tree_pos;
vi tree_weight;
ll solve(int mask)
{
	if (mask == (1 << n) - 1) return 0;
	ll& v = dp[mask];
	if (v != -1) return v;
	ll ret = inf;

	rep(i, n) rep(j, n)
	{
		if (mask & (1 << i)) continue;
		if (mask & (1 << j)) continue;
		if (tree_weight[i] + tree_weight[j] > K) continue;

		ret = min(ret, 2 * max({ tree_pos[i], tree_pos[j] }) + solve(mask | (1 << i) | (1 << j)));
	}

	rep(i, n)
	{
		if (mask & (1 << i)) continue;
		assert(tree_weight[i] <= K);

		ret = min(ret, 2 * tree_pos[i] + solve(mask | (1 << i)));
	}

	return v=ret;
}

int main()
{
	cin.tie(0)->sync_with_stdio(0);

	memset(dp, -1, sizeof(dp));

	cin >> n >> K;
	tree_pos.resize(n);
	tree_weight.resize(n);
	repe(p, tree_pos) cin >> p;
	repe(w, tree_weight) cin >> w;

	cout << 2*n+solve(0) << '\n';

	return 0;
}
