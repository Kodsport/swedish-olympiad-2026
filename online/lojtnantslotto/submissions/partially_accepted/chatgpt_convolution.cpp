#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (ll)9e18;

// min-plus convolution where `a` is convex and `b` arbitrary.
// returns c of length n+m-1, where c[h] = min_{t} a[t] + b[h-t]
vector<ll> minplus_conv_convex_first(const vector<ll>& a, const vector<ll>& b){
    int n = (int)a.size();
    int m = (int)b.size();
    if(n == 0 || m == 0) return {};
    int z = n + m - 1;
    vector<ll> c(z, INF);

    // divide-and-conquer optimization assuming argmin is monotone
    function<void(int,int,int,int)> dc = [&](int L, int R, int optL, int optR){
        if(L > R) return;
        int mid = (L + R) >> 1;
        int tlo = max(0, mid - (m - 1));
        int thi = min(n - 1, mid);
        int start = max(tlo, optL);
        int end   = min(thi, optR);
        ll bestVal = INF;
        int bestT = start;
        for(int t = start; t <= end; ++t){
            ll val = a[t] + b[mid - t];
            if(val < bestVal){
                bestVal = val;
                bestT = t;
            }
        }
        c[mid] = bestVal;
        if(L <= mid-1) dc(L, mid-1, optL, bestT);
        if(mid+1 <= R) dc(mid+1, R, bestT, optR);
    };

    dc(0, z-1, 0, n-1);
    return c;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, T;
    if(!(cin >> N >> T)) return 0;
    vector<vector<int>> groups(6); // 1..5
    for(int i=0;i<N;i++){
        int s,g;
        cin >> s >> g;
        if(s < 1 || s > 5) continue;
        if(g > T) {
            // if g > T we can never spend that many troops, but still keep it:
            // actually we can safely keep them because their prefix will exceed T anyway
            // but skipping them reduces work; however keep them is safe.
            // we'll push them but they will not contribute.
        }
        groups[s].push_back(g);
    }

    // prepare prefix sums (a[k] = cost to take k items from this group)
    vector<vector<ll>> prefix(6);
    for(int s = 1; s <= 5; ++s){
        auto &v = groups[s];
        sort(v.begin(), v.end());
        prefix[s].resize(v.size()+1);
        prefix[s][0] = 0;
        for(size_t i=0;i<v.size();++i){
            prefix[s][i+1] = prefix[s][i] + v[i];
            if(prefix[s][i+1] > (ll)T + (ll)1e12) prefix[s][i+1] = INF; // guard overflow
        }
    }

    // dp[v] = minimal cost to reach value v
    vector<ll> dp(1, 0); // dp[0] = 0

    for(int s = 1; s <= 5; ++s){
        int cnt = (int)groups[s].size();
        if(cnt == 0) continue;

        // new dp size upper bound:
        int oldV = (int)dp.size() - 1;
        int newV = oldV + cnt * s;
        vector<ll> dp_new(newV + 1, INF);

        // for each residue class modulo s do convolution
        for(int r = 0; r < s; ++r){
            // build b: b[t] = dp[r + t*s]
            vector<ll> b;
            for(int idx = r; idx <= oldV; idx += s){
                b.push_back(dp[idx]);
            }
            if(b.empty()) continue;

            // a = prefix[s] (length cnt+1), convex
            vector<ll> a(prefix[s].begin(), prefix[s].end());

            vector<ll> c = minplus_conv_convex_first(a, b);
            // write back: dp_new[r + h*s] = min(dp_new[...], c[h])
            for(int h = 0; h < (int)c.size(); ++h){
                int pos = r + h * s;
                if(pos <= newV){
                    dp_new[pos] = min(dp_new[pos], c[h]);
                }
            }
        }

        // Trim dp_new by discarding values with cost INF beyond possible bound
        // But we keep full vector for correctness (we will later only scan for dp[v] <= T)
        dp.swap(dp_new);
    }

    // find largest v with dp[v] <= T
    int ans = 0;
    for(int v = 0; v < (int)dp.size(); ++v){
        if(dp[v] <= T) ans = v;
    }
    cout << ans << "\n";
    return 0;
}
