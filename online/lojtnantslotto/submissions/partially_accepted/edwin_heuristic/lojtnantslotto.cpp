#include <algorithm>
#include <climits>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

typedef unsigned long long ull;
typedef long long ll;

#define V_SUM_MAX 25000
#define N_MAX 5000
#define W_MAX 1000000000

// Initialize dp array
int dp[V_SUM_MAX + 1][N_MAX + 1];

// Function to return the maximum weight
int maxWeight(vector<int> &w, vector<int> &val, int n, int c) {
	for (int i = 0; i <= V_SUM_MAX; i++)
		for (int j = 0; j <= n; j++)
			dp[i][j] = W_MAX;

	// Base case initialization
	for (int i = 0; i <= n; i++)
		dp[0][i] = 0;

	// iterate over subproblems ans get
	// the current value from previous computation
	for (int i = 1; i <= V_SUM_MAX; i++)
		for (int j = 1; j <= n; j++)
			dp[i][j] =
				min(dp[i][j - 1], (i >= val[j - 1])
									  ? w[j - 1] + dp[i - val[j - 1]][j - 1]
									  : W_MAX);

	// Finding maximum value
	for (int i = V_SUM_MAX; i >= 0; i--)
		if (dp[i][n] <= c)
			return i;

	return 0;
}

// Comparison function to sort items based on value/weight ratio
bool compare(vector<int> &a, vector<int> &b) {
	double a1 = (1.0 * a[0]) / a[1];
	double b1 = (1.0 * b[0]) / b[1];
	return a1 > b1;
}

double fractionalKnapsack(vector<int> &val, vector<int> &wt, int capacity) {
	int n = val.size();

	// Create 2D vector to store value and weight
	// items[i][0] = value, items[i][1] = weight
	vector<vector<int>> items(n, vector<int>(2));

	for (int i = 0; i < n; i++) {
		items[i][0] = val[i];
		items[i][1] = wt[i];
	}

	// Sort items based on value-to-weight ratio in descending order
	sort(items.begin(), items.end(), compare);

	double res = 0.0;
	int currentCapacity = capacity;

	// Process items in sorted order
	for (int i = 0; i < n; i++) {

		// If we can take the entire item
		if (items[i][1] <= currentCapacity) {
			res += items[i][0];
			currentCapacity -= items[i][1];
		}

		// Otherwise take a fraction of the item
		else {
			res += (1.0 * items[i][0] / items[i][1]) * currentCapacity;

			// Knapsack is full
			break;
		}
	}

	return res;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	int castleCount, soldierCount;
	cin >> castleCount >> soldierCount;
	vector<int> w;
	vector<int> val;
	bool useFast = true;
	for (int i = 0; i < castleCount; ++i) {
		int a, b;
		cin >> a >> b;
		val.push_back(a);
		w.push_back(b);
		if (a > 2) {
			useFast = false;
		}
	}
	if (useFast || castleCount > 5000) {
		cout << (int)fractionalKnapsack(val, w, soldierCount) << endl;
	} else {
		cout << maxWeight(w, val, castleCount, soldierCount) << endl;
	}
	return 0;
}
