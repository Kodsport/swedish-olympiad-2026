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


const ll offset = 100;
const int max_coord = 100;
char vis[max_coord + offset][max_coord + offset];
char covered[max_coord + offset][max_coord + offset];

int main()
{
	cin.tie(0)->sync_with_stdio(0);

	ll r, c;
	cin >> r >> c;

	vector<string> grid(r);
	repe(row, grid) cin >> row;

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
	auto in_grid = [&](ll a, ll b)
	{
		return a + offset >= 0 && a < max_coord && b + offset >= 0 && b < max_coord;
	};

	auto solve = [&](p2 start, p2 goal)
	{
		if (start == goal) return true;
		assert(start.first >= 0 && start.second >= 0);
		memset(covered, 0, sizeof(covered));
		assert(!is_blocked(start.first, start.second));
		vector<p2> dirs = { {0,1},{0,-1},{1,0},{-1,0} };

		auto bfs = [&](p2 st)
		{
			memset(vis, 0, sizeof(vis));
			queue<p2> q;
			q.emplace(p2(st.first % r, st.second % c));
			ll num_edge = 0;
			while (sz(q))
			{
				p2 p = q.front();
				q.pop();

				if (vis[p.first + offset][p.second + offset]) continue;
				vis[p.first + offset][p.second + offset] = 1;

				if (!in_grid(p.first + 1, p.second) || !in_grid(p.first, p.second + 1)
					|| !in_grid(p.first - 1, p.second) || !in_grid(p.first, p.second - 1)) num_edge++;

				repe(dir, dirs)
				{
					p2 np = p2(p.first + dir.first, p.second + dir.second);
					if (is_blocked(np.first, np.second)) continue;
					if (!in_grid(np.first, np.second)) continue;
					q.push(np);
				}
			}
			return num_edge;
		};

		ll num_edge = bfs(start);
		if (num_edge > 50)
		{
			if (bfs(goal) > 50) return true; // we are part of one huge component
			return false; // goal is in isolated component
		}
		if (num_edge == 0)
		{
			p2 moves = { start.first / r * r, start.second / c * c };
			assert(start.first - moves.first == start.first % r);
			goal.first -= moves.first;
			goal.second -= moves.second;
			if (!in_grid(goal.first, goal.second)) return false;
			return vis[goal.first + offset][goal.second + offset] == 1;
		}

		auto rec_valid = [&](p2 start, p2 off)
		{
			int count = 0;
			rep(k, 2000)
			{
				p2 p = p2(start.first + off.first * k, start.second + off.second * k);
				if (is_blocked(p.first, p.second)) return false;
				if (!in_grid(p.first, p.second)) break;
				count++;
				if (!vis[p.first + offset][p.second + offset]) return false;
			}
			if (count == 0) return false;
			rep(k, 2000)
			{
				p2 p = p2(start.first + off.first * k, start.second + off.second * k);
				if (!in_grid(p.first, p.second)) break;
				covered[p.first + offset][p.second + offset] = true;
			}
			return true;
			assert(false);
		};

		vector<pair<p2, p2>> recs;
		repp(i, -r * 2, r * 2) repp(j, -c * 2, c * 2)
		{
			repp(a, -20, 20) repp(b, -20, 20) if (rec_valid(p2(i, j), p2(a, b))) recs.emplace_back(p2(i, j), p2(a, b));
		}

		p2 moves = { start.first / r * r, start.second / c * c };
		assert(start.first - moves.first == start.first % r);
		goal.first -= moves.first;
		goal.second -= moves.second;
		for (auto [st, off] : recs)
		{
			p2 d = p2(goal.first - st.first, goal.second - st.second);
			if (off.first == 0 && off.second == 0)
			{
				if (d == p2(0, 0)) return true;
			}
			else if (off.first == 0 && off.second != 0)
			{
				if (d.first == 0 && d.second % off.second == 0) return true;
			}
			else if (off.first != 0 && off.second == 0)
			{
				if (d.first % off.first == 0 && d.second == 0) return true;
			}
			else
			{
				if (d.first % off.first == 0 && d.second % off.second == 0 && d.first / off.first == d.second / off.second)
				{
					return true;
				}
			}
		}
		return false;
	};

	rep(i, 5)
	{
		ll r1, c1, r2, c2;
		cin >> r1 >> c1 >> r2 >> c2;

		cout << (solve(p2(r1, c1), p2(r2, c2)) ? "Ja" : "Nej") << '\n';
	}


	return 0;
}
