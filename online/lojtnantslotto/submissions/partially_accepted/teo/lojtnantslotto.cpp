#include <bits/stdc++.h>
using namespace std;

vector<int> worth, defense;

// https://stackoverflow.com/questions/7949705/variation-on-knapsack-minimum-total-value-exceeding-w
//

int main() {
  int n, t;
  cin >> n >> t;

  worth.resize(n);
  defense.resize(n);
  for (int i = 0; i < n; i++)
    cin >> worth[i] >> defense[i];

  vector<vector<int>> by_value(6);
  for (int i = 0; i < n; i++)
    by_value[worth[i]].push_back(defense[i]);

  for (int i = 1; i <= 5; i++)
    sort(by_value[i].begin(), by_value[i].end());

  vector<array<int, 6>> locs;
  vector<int> left, sums;

  locs.push_back({0, 0, 0, 0, 0, 0});
  left.push_back(t);
  sums.push_back(0);

  int ans = 0;
  for (int i = 1; i < 5 * n; i++) {
    array<int, 6> best_locs;
    int best_left = -1;
    int best_sum = 0;

    for (int j = 1; j <= min(i, 5); j++) {
      if (by_value[j].size() <= locs[i - j][j])
        continue;
      if (left[i - j] == -1)
        continue;
      int new_left = left[i - j] - by_value[j][locs[i - j][j]];
      int new_sum = sums[i - j] + 1;
      if (best_left < new_left ||
          (best_left == new_left && best_sum > new_sum)) {
        best_locs = locs[i - j];
        best_locs[j]++;
        best_left = new_left;
        best_sum = new_sum;
      }
    }

    if (0 <= best_left)
      ans = i;

    locs.push_back(best_locs);
    left.push_back(best_left);
    sums.push_back(best_sum);
  }

  cout << ans << endl;

  return 0;
}
