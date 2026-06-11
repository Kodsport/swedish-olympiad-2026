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

const int maxval = 5;

struct Item {
    ll val;
    ll cost;
    int cat;
};

int main() {
    cin.tie(0)->sync_with_stdio(0);

    auto start = chrono::high_resolution_clock::now();

    int n, t;
    cin >> n >> t;
    vector<p2> castles(n);
    repe(c, castles) cin >> c.first >> c.second;

    vvi occs(maxval + 1);
    for (auto [profit, weight] : castles) {
        occs[profit].push_back(weight);
    }
    repe(o, occs) sort(all(o));

    vvi occs_pref = occs;
    rep(i, maxval + 1) {
        repp(j, 1, sz(occs_pref[i])) {
            occs_pref[i][j] += occs_pref[i][j - 1];
        }
    }

    mt19937 rng(42);

    // 1. Generate an initial greedy baseline solution
    vector<ll> cur_k(maxval + 1, 0);
    vector<Item> items;
    repp(i, 1, maxval + 1) {
        rep(j, sz(occs[i])) {
            items.push_back({i, occs[i][j], (int)i});
        }
    }
    sort(all(items), [](const Item& a, const Item& b) {
        return a.val * b.cost > b.val * a.cost;
    });

    ll temp_t = t;
    repe(item, items) {
        if (temp_t >= item.cost) {
            temp_t -= item.cost;
            cur_k[item.cat]++;
        }
    }

    vector<ll> best_k = cur_k;
    ll best_profit = 0;
    repp(i, 1, maxval + 1) best_profit += i * best_k[i];

    ll cur_profit = best_profit;

    // 2. Local search with random restarts and mutations
    int iter = 0;
    while (1) {
        if (iter % 1000 == 0) {
            auto end = chrono::high_resolution_clock::now();
            if (chrono::duration_cast<chrono::milliseconds>(end - start).count() > 950) {
                break;
            }
        }
        iter++;

        vector<ll> next_k = cur_k;

        // Occasionally restart or jump to the best known state
        if (rng() % 100 < 5) {
            if (rng() % 2 == 0) {
                next_k = best_k;
            } else {
                next_k.assign(maxval + 1, 0);
                ll rem_budget = t;
                vector<int> order = {1, 2, 3, 4, 5};
                shuffle(all(order), rng);
                for (int i : order) {
                    if (occs_pref[i].empty()) continue;
                    auto it = upper_bound(all(occs_pref[i]), rem_budget);
                    int max_k = distance(occs_pref[i].begin(), it);
                    if (max_k > 0) {
                        int chosen_k = uniform_int_distribution<int>(0, max_k)(rng);
                        next_k[i] = chosen_k;
                        if (chosen_k > 0) rem_budget -= occs_pref[i][chosen_k - 1];
                    }
                }
            }
        }

        // Mutation: decrement a random category
        int dec_idx = rng() % maxval + 1;
        if (next_k[dec_idx] > 0) {
            int dec_amt = uniform_int_distribution<int>(1, next_k[dec_idx])(rng);
            next_k[dec_idx] -= dec_amt;
        }

        // Calculate remaining budget
        ll used_budget = 0;
        repp(i, 1, maxval + 1) {
            if (next_k[i] > 0) {
                used_budget += occs_pref[i][next_k[i] - 1];
            }
        }
        ll rem_budget = t - used_budget;

        // Greedily fill the remaining budget in random order
        vector<int> order = {1, 2, 3, 4, 5};
        shuffle(all(order), rng);
        for (int i : order) {
            if (occs_pref[i].empty()) continue;
            ll current_cost = (next_k[i] > 0 ? occs_pref[i][next_k[i] - 1] : 0);
            ll target_cost = rem_budget + current_cost;
            auto it = upper_bound(all(occs_pref[i]), target_cost);
            int new_k = distance(occs_pref[i].begin(), it);
            ll added_cost = (new_k > 0 ? occs_pref[i][new_k - 1] : 0) - current_cost;
            rem_budget -= added_cost;
            next_k[i] = new_k;
        }

        ll next_profit = 0;
        repp(i, 1, maxval + 1) next_profit += i * next_k[i];

        // Simulated annealing-like acceptance criteria
        if (next_profit > cur_profit) {
            cur_k = next_k;
            cur_profit = next_profit;
        } else if (next_profit == cur_profit && rng() % 100 < 50) {
            cur_k = next_k;
        } else if (rng() % 1000 < 10) { 
            cur_k = next_k;
            cur_profit = next_profit;
        }

        if (cur_profit > best_profit) {
            best_profit = cur_profit;
            best_k = cur_k;
        }
    }

    cout << best_profit << '\n';

    return 0;
}
