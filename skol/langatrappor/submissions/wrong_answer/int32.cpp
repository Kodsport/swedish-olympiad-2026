// The right algorithm with a 32-bit accumulator.  A full-height column at
// f_i = 10^9 sums to ~5*10^17, so the *-overflow cases (and most large-R cases)
// blow straight past 2^31.  This is the mistake the statement warns about in bold.
#include <bits/stdc++.h>
using namespace std;

int R, C;
vector<int> F;
int sx, sy, gx, gy;

int height(int r, int c) { return F[c] - r; }

int horisontal(int c1, int c2, int r) {
	if (c1 > c2) swap(c1, c2);
	int res = 0;
	for (int c = c1; c <= c2; c++) res += height(r, c);
	return res;
}

int vertical(int c, int r1, int r2) {
	if (r1 > r2) swap(r1, r2);
	int d = r2 - r1 + 1;
	return d * F[c] - d * r1 - (d * d - d) / 2;
}

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin >> R >> C;
	F.resize(C);
	for (int &v : F) cin >> v;
	cin >> sx >> sy >> gx >> gy;
	if (sy < gy) { swap(sx, gx); swap(sy, gy); }

	int ans = INT_MAX;
	for (int i = 0; i < C; i++) {
		int cur = vertical(i, sy, gy) + horisontal(sx, i, sy)
		        + horisontal(gx, i, gy) - height(sy, i) - height(gy, i);
		ans = min(ans, cur);
	}
	for (int c1 = 0; c1 < C; c1++) {
		for (int c2 = 0; c2 < C; c2++) {
			int cur = horisontal(sx, c1, sy) + vertical(c1, sy, R - 1)
			        + horisontal(c1, c2, R - 1) + vertical(c2, R - 1, gy)
			        + horisontal(c2, gx, gy);
			cur -= height(sy, c1) + height(R - 1, c1)
			     + height(R - 1, c2) + height(gy, c2);
			ans = min(ans, cur);
		}
	}
	cout << ans << '\n';
	return 0;
}
