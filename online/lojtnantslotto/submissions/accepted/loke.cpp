#pragma GCC optimize ("O3")
#include <bits/allocator.h>
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, ll> pii;
typedef vector<ll> vi;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
#define rall(x) rbegin(x), rend(x)
#define sz(x) (int)(x).size()

void printvi(string label, vi& v) {
  cerr << label<<":";
  rep(i, 0, min(sz(v), 20)) cerr << " " << v[i];
  cerr << endl;
}

vi min_plus_convolution_approx_convex(vi cost_by_profit1, vi cost_by_profit2) {
  assert(sz(cost_by_profit1) == sz(cost_by_profit2));
  int P = sz(cost_by_profit1);
  vi ret(P, 1e9);

  int profit1_init = 0;
  constexpr int MARGIN = 20;
  vi todo;
  rep(tot_profit, 0, P) {
    int lo = max(profit1_init-MARGIN, 0);
    int hi = min(profit1_init+MARGIN, tot_profit);
    todo.clear();
    rep(i, lo, hi+1) todo.push_back(i);
    int tot_todo = 0;
    while (sz(todo) > 0) {
      tot_todo++;
      assert(tot_todo < 1000);
      int profit1 = todo[sz(todo)-1];
      todo.pop_back();
      if (ret[tot_profit] > cost_by_profit1[profit1] + cost_by_profit2[tot_profit - profit1]) {
        ret[tot_profit] = cost_by_profit1[profit1] + cost_by_profit2[tot_profit - profit1];
        profit1_init = profit1;
        while (max(profit1-MARGIN, 0) < lo) {
          lo--; todo.push_back(lo);
        }
        while (min(profit1+MARGIN, tot_profit) > hi) {
          hi++; todo.push_back(hi);
        }
      }
    }
  }
  /*
  cerr << endl;
  printvi("cost_by_profit1", cost_by_profit1);
  printvi("cost_by_profit2", cost_by_profit2);
  printvi("cost_by_profitR", ret);
  */
  return ret;
}

int main() {
  cin.sync_with_stdio(0); cin.tie(0);
  cin.exceptions(cin.failbit);

  int N, T; cin >> N >> T;
  vector<vi> castle_costs_by_profit(5);
  rep(i, 0, N) {
    ll profit, cost; cin >> profit >> cost;
    castle_costs_by_profit[profit-1].push_back(cost);
  }
  int MAX_PROFIT = N*5;
  vector<vi> costs_by_profits(5, vi(MAX_PROFIT+1, 1e9));
  rep(i, 0, 5) {
    int p = i+1;
    sort(all(castle_costs_by_profit[i]));
    costs_by_profits[i][0] = 0;
    rep(j, 0, sz(castle_costs_by_profit[i])) {
      rep(k, 0, p) {
        if (j*p+k+1 >= sz(costs_by_profits[i])) goto next;
        costs_by_profits[i][j*p+k+1] = castle_costs_by_profit[i][j];
        if (j > 0) costs_by_profits[i][j*p+k+1] += costs_by_profits[i][(j-1)*p+k+1];
      }
    }
next:
    1;
    //cerr << "profit="<<(i+1)<<" costs for increasing profit:";
    //rep(j, 0, 12) cerr << " " << costs_by_profits[i][j];
    //cerr << endl;
  }

  vi cost_by_profit = costs_by_profits[0];
  cost_by_profit = min_plus_convolution_approx_convex(cost_by_profit, costs_by_profits[1]);
  cost_by_profit = min_plus_convolution_approx_convex(cost_by_profit, costs_by_profits[2]);
  cost_by_profit = min_plus_convolution_approx_convex(cost_by_profit, costs_by_profits[3]);
  cost_by_profit = min_plus_convolution_approx_convex(cost_by_profit, costs_by_profits[4]);

  int ans = 0;
  rep(profit, 0, MAX_PROFIT+1) {
    if (cost_by_profit[profit] <= T) ans = max(ans, profit);
  }
  cout << ans << endl;
}
