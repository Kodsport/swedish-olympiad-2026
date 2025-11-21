#include "validator.h"
using namespace std;

void run() {
	int max_sidelen = Arg("maxr", 5);
	int max_coord = Arg("max_coord", int(1e9));

	int r = Int(1, max_sidelen);
	Space();
	int c = Int(1, max_sidelen);
	Endl();

	vector<string> grid(r);
	for (int i = 0; i < r; i++) {
		grid[i] = Line();
		assert(grid[i].size() == c);
		for (char ch : grid[i]) {
			assert(ch == '.' || ch == '#');
		}
	}
	reverse(grid.begin(), grid.end());

	auto is_blocked = [&](int row, int col) {
		return grid[row % r][col % c] == '#';
	};

	for (int i = 0; i < 5; i++) {
		int c1 = Int(0, max_coord);
		Space();
		int r1 = Int(0, max_coord);
		Space();
		int c2 = Int(0, max_coord);
		Space();
		int r2 = Int(0, max_coord);
		Endl();

		assert(!is_blocked(r1, c1));
		assert(!is_blocked(r2, c2));
	}
}
