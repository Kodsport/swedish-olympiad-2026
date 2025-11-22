// O(R), but runs in under 1 second
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
    assert(c==2);
	vi start_val(c);
	repe(v, start_val) cin >> v;
	p2 start, goal;
	cin >> start.second >> start.first >> goal.second >> goal.first;

	auto cost_at = [&](p2 p)
	{
		return start_val[p.second] - p.first;
	};

	ll base_cot = cost_at(start);
	base_cot += cost_at(goal);

	ll colcost = inf;
	if (start.second == goal.second)
	{
		colcost = 0;
		repp(j, min(start.first, goal.first) + 1, max(start.first, goal.first)) colcost += cost_at(p2(j, start.second));
	}
	else
	{
		if (start.first > goal.first) swap(goal.first, start.first);
		assert(start.first <= goal.first);
		ll cand = 0;
		repp(j, start.first + 1, goal.first+1) cand += cost_at(p2(j, start.second));
		colcost = min(colcost, cand);

		cand = 0;
		repp(j, start.first, goal.first) cand += cost_at(p2(j, goal.second));
		colcost = min(colcost, cand);
	}
	cout << colcost+base_cot << '\n';

	return 0;
}
