#include "validator.h"

using namespace std;

void run() {
	int n = Int(1, 8);
	Space();
	int k = Int(20, 50);
	Endl();

	if (Arg("k_25", 0)) assert(k == 25);

	vector<int> coords = SpacedInts(n, 1, 30);

	if (Arg("same_coord", 0)) {
		for (int i = 0; i < n; i++) {
			assert(coords[i] = coords[0]);
		}
	}

	bool weight_20 = Arg("weight_20", 0);
	vector<int> weights = SpacedInts(n, 20, k);
	for (int w : weights) {
		assert(w <= k);
		assert(w == 20 || w == 40);
		if (weight_20) assert(w == 20);
	}
}
