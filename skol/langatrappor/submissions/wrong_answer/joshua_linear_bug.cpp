// double counts a little
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

	ll r, c;
	cin >> r >> c;
	vi start_val(c);
	repe(v, start_val) cin >> v;
	p2 start, goal;
	cin >> start.second >> start.first >> goal.second >> goal.first;

	if (start.first < goal.first)
	{
		swap(start, goal);
	}
	if (start.second < goal.second)
	{
		start.second = c - 1 - start.second;
		goal.second = c - 1 - goal.second;
		reverse(all(start_val));
	}
	assert(start.first >= goal.first);
	assert(start.second >= goal.second);


	auto cost_at = [&](p2 p)
	{
		return start_val[p.second] - p.first;
	};

	
	auto col_dist = [&](ll a, ll b, ll row)
	{
		ll dist = 0;
		repp(j, min(a, b), max(a, b) + 1) dist += cost_at(p2(row, j));
		return dist;
	};
	ll startcol = start.second;
	ll goalcol = goal.second;
	ll ans = inf;
	rep(i, c)
	{
		ll cost = col_dist(i,startcol,start.first)+col_dist(i,goalcol,goal.first);
		
		repp(j, goal.first, start.first + 1)
		{
			cost += cost_at(p2(j, i));
		}

		ans = min(ans, cost);
	}
	cout << ans << '\n';

	return 0;
}
