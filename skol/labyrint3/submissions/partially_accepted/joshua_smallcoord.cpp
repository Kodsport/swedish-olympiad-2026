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


const int offset = 100;
const int max_coord = 1000;
int vis[max_coord + offset][max_coord + offset];

int main()
{
	cin.tie(0)->sync_with_stdio(0);

	int r, c;
	cin >> r >> c;

	vector<string> grid(r);
	repe(row, grid) cin >> row;

	auto mod = [](int a, int b)
	{
		a %= b;
		a += b;
		a %= b;
		return a;
	};

	auto is_blocked = [&](int a, int b)
	{
		return grid[mod(a, r)][mod(b, c)] == '#';
	};

	auto in_grid = [&](int a, int b)
	{
		return a + offset >= 0 && a + offset < max_coord && b + offset >= 0 && b + offset < max_coord;
	};

	vector<p2> dirs = { {0,1},{0,-1},{1,0},{-1,0} };
	rep(i, 5)
	{
		memset(vis, 0, sizeof(vis));

		int r1, c1, r2, c2;
		cin >> r1 >> c1 >> r2 >> c2;
		
		bool found = false;
		queue<p2> q;
		q.emplace(r1, c1);
		while (sz(q))
		{
			p2 p = q.front();
			q.pop();

			if (vis[p.first + offset][p.second + offset]) continue;
			vis[p.first + offset][p.second + offset] = 1;

			if (p == p2(r2,c2))
			{
				found = true;
				break;
			}

			repe(dir, dirs)
			{
				p2 np = p2(p.first + dir.first, p.second + dir.second);
				if (!in_grid(np.first, np.second)) continue;
				if (is_blocked(np.first, np.second)) continue;
				q.push(np);
			}
		}

		cout << (found ? "Ja" : "Nej") << '\n';
	}


	return 0;
}
