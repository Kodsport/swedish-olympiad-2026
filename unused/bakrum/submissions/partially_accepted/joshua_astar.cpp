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


struct node_hash {
	std::size_t operator()(const p2& _node) const {
		return _node.first * 1e9 + _node.second;
	}
};

int main()
{
	cin.tie(0)->sync_with_stdio(0);

	ll r, c;
	cin >> r >> c;

	vector<string> grid(r);
	repe(row, grid) cin >> row;
	reverse(all(grid));

	auto mod = [](ll a, ll b)
	{
		a %= b;
		a += b;
		a %= b;
		return a;
	};

	auto is_blocked = [&](ll a, ll b)
	{
		return grid[mod(a, r)][mod(b, c)] == '#';
	};

	auto dist = [](p2 a, p2 b)
	{
		return abs(a.first - b.first) + abs(a.second - b.second);
	};

	vector<p2> dirs = { {0,1},{0,-1},{1,0},{-1,0} };
	unordered_set<p2, node_hash> seen;
	seen.reserve(3e6);
	rep(i, 5)
	{
		ll r1, c1, r2, c2;
		cin >> c1 >> r1 >> c2 >> r2;
		seen.clear();

		bool found = false;
		priority_queue<pair<ll, p2>> q;
		p2 goal = { r2,c2 };
		q.emplace(0, p2(r1, c1));
		while (sz(q))
		{
			auto [d, p] = q.top();
			q.pop();

			if (sz(seen) > 1e6) break;

			if (p == goal)
			{
				found = true;
				break;
			}

			repe(dir, dirs)
			{
				p2 np = p2(p.first + dir.first, p.second + dir.second);
				if (is_blocked(np.first, np.second)) continue;
                if (seen.count(np)) continue;
                seen.insert(np);
				q.emplace(-dist(np,goal), np);
			}
		}

		cout << (found ? "Ja" : "Nej") << '\n';
	}


	return 0;
}
