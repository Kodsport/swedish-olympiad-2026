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

	int r, c;
	cin >> r >> c;
	vector<string> grid(r);
	repe(row, grid) cin >> row;
	reverse(all(grid));

	int block_cnt = 0;
	rep(i, r) rep(j, c) block_cnt += grid[i][j] == '#';
	assert(block_cnt <= 3);

	auto answer = [&](p2 start, p2 goal)
	{
		if (block_cnt <= 1) return true;
		if (block_cnt == 3) return start == goal;

		vector<p2> blocked;
		rep(i, r) rep(j, c) if (grid[i][j] == '#') blocked.emplace_back(i, j);
		assert(sz(blocked) == 2);
		p2 a = blocked[0];
		p2 b = blocked[1];
		if (a.first == b.first)
		{
			return start.first == goal.first;
		}
		else if (a.second == b.second)
		{
			return start.second == goal.second;
		}
		else // diagonal
		{
			return start == goal;
		}
	};

	rep(i, 5)
	{
		int r1, c1, r2, c2;
		cin >> c1 >> r1 >> c2 >> r2;
		cout << (answer(p2(r1, c1), p2(r2, c2)) ? "Ja" : "Nej") << '\n';
	}

	return 0;
}
