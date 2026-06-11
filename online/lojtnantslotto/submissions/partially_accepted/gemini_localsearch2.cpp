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

int main()
{
    cin.tie(0)->sync_with_stdio(0);

    auto start = chrono::high_resolution_clock::now();

    int n, t;
    if (!(cin >> n >> t)) return 0;
    vector<p2> castles(n);
    repe(c, castles) cin >> c.first >> c.second;

    vvi occs(maxval + 1);
    for (auto [profit, weight] : castles) occs[profit].push_back(weight);
    repe(o, occs) sort(all(o));
    
    vvi occs_pref = occs;
    rep(i, maxval + 1) repp(j, 1, sz(occs_pref[i])) occs_pref[i][j] += occs_pref[i][j - 1];
    
    mt19937 rng(42);
    uniform_real_distribution<double> dist(0.0, 1.0);

    ll ans = 0;
    int iters = 0;
    
    while (1) {
        // Time check every 128 iterations to reduce syscall overhead
        if ((iters & 127) == 0) {
            auto now = chrono::high_resolution_clock::now();
            // Stop just before 2.0s limit (adjust to ~950 if strict 1.0s limit)
            if (chrono::duration_cast<chrono::milliseconds>(now - start).count() > 1950) break;
        }
        iters++;

        // Randomly generate proportions for the castles picked per profit group
        double f[6];
        bool all_zero = true;
        for(int i = 1; i <= maxval; i++) {
            if (sz(occs_pref[i]) == 0) {
                f[i] = 0;
            } else {
                f[i] = dist(rng);
                if (f[i] > 0) all_zero = false;
            }
        }
        if (all_zero) continue;

        // Binary search the multiplier to move along the random proportion "ray"
        int low = 0, high = 10000000, best_M = 0;
        while(low <= high) {
            int mid = low + (high - low) / 2;
            ll w = 0;
            for(int i = 1; i <= maxval; i++) {
                int c = min((ll)sz(occs_pref[i]), (ll)(mid * f[i]));
                if (c > 0) w += occs_pref[i][c - 1];
            }
            if(w <= t) {
                best_M = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        // Collect state directly at binary searched border
        int c[6];
        ll cur_w = 0, cur_p = 0;
        for(int i = 1; i <= maxval; i++) {
            c[i] = min((ll)sz(occs_pref[i]), (ll)(best_M * f[i]));
            if (c[i] > 0) cur_w += occs_pref[i][c[i] - 1];
            cur_p += c[i] * i;
        }

        // Local Localizer Extender: greedily perfect local area mapped to border point
        while(true) {
            // Step 1: Greedily add anything that fits loosely
            while(true) {
                int best_i = -1;
                double best_ratio = -1.0;
                for(int i = 1; i <= maxval; i++) {
                    if (c[i] < sz(occs_pref[i])) {
                        ll cost = occs_pref[i][c[i]] - (c[i] > 0 ? occs_pref[i][c[i]-1] : 0);
                        if (cur_w + cost <= t) {
                            double ratio = (double)i / cost;
                            if (ratio > best_ratio) {
                                best_ratio = ratio;
                                best_i = i;
                            }
                        }
                    }
                }
                if (best_i != -1) {
                    ll cost = occs_pref[best_i][c[best_i]] - (c[best_i] > 0 ? occs_pref[best_i][c[best_i]-1] : 0);
                    cur_w += cost;
                    cur_p += best_i;
                    c[best_i]++;
                } else {
                    break;
                }
            }

            // Step 2: Swap phase - trade a cheaper item for a costlier/better one safely
            int best_i = -1, best_j = -1;
            int best_profit_inc = 0;
            ll best_weight_inc = inf;

            for(int i = 1; i <= maxval; i++) {
                if (c[i] == 0) continue;
                ll cost_i = occs_pref[i][c[i]-1] - (c[i] > 1 ? occs_pref[i][c[i]-2] : 0);
                
                for(int j = i + 1; j <= maxval; j++) {
                    if (c[j] == sz(occs_pref[j])) continue;
                    ll cost_j = occs_pref[j][c[j]] - (c[j] > 0 ? occs_pref[j][c[j]-1] : 0);
                    
                    if (cur_w - cost_i + cost_j <= t) {
                        int profit_inc = j - i;
                        ll weight_inc = cost_j - cost_i;
                        if (profit_inc > best_profit_inc || (profit_inc == best_profit_inc && weight_inc < best_weight_inc)) {
                            best_profit_inc = profit_inc;
                            best_weight_inc = weight_inc;
                            best_i = i;
                            best_j = j;
                        }
                    }
                }
            }
            if (best_i != -1) {
                ll cost_i = occs_pref[best_i][c[best_i]-1] - (c[best_i] > 1 ? occs_pref[best_i][c[best_i]-2] : 0);
                ll cost_j = occs_pref[best_j][c[best_j]] - (c[best_j] > 0 ? occs_pref[best_j][c[best_j]-1] : 0);
                cur_w = cur_w - cost_i + cost_j;
                cur_p += best_profit_inc;
                c[best_i]--;
                c[best_j]++;
            } else {
                break;
            }
        }
        
        if (cur_p > ans) ans = cur_p;
    }

    cout << ans << '\n';

    return 0;
}
