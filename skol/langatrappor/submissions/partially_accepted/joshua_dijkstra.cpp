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

	auto cost_at = [&](p2 p)
	{
		return start_val[p.second] - p.first;
	};

	priority_queue<pair<ll, p2>> pq;
	pq.emplace(-cost_at(start), start);

	set<p2> vis;
	vector<p2> dirs = { {0,1},{0,-1},{1,0},{-1,0} };
	while (sz(pq))
	{
		auto [d, p] = pq.top();
		pq.pop();

		if (vis.count(p)) continue;
		vis.insert(p);

		if (p==goal)
		{
			cout << -d << "\n";
			return 0;
		}

		repe(dir, dirs)
		{
			p2 np = p2(p.first + dir.first, p.second + dir.second);
			if (np.first < 0 || np.second < 0 || np.first >= r || np.second >= c) continue;
			pq.emplace(d - cost_at(np), np);
		}
	}

	assert(0);

	return 0;
}
