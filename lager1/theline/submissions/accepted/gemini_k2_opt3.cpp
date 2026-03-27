#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Data structure for the Envelope's sweep line
struct Event1D {
    long long pos;
    int delta_slope;
    bool operator<(const Event1D& o) const {
        if (pos != o.pos) return pos < o.pos;
        return delta_slope < o.delta_slope;
    }
};

// Data structure for the specific `y` candidate sweep line
struct Event {
    long long pos;
    int w;
};

int main() {
    // Optimize standard I/O operations for competitive programming
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);
    
    int N_hubs, K;
    if (!(cin >> N_hubs >> K)) return 0;
    
    vector<long long> S(K), T(K);
    long long original_dist = 0;
    vector<Event1D> env_events;
    
    for (int i = 0; i < K; i++) {
        long long s, t; 
        cin >> s >> t;
        if (s > t) swap(s, t);
        S[i] = s; 
        T[i] = t;
        original_dist += (t - s);
        
        long long C = (t - s) - 1;
        if (C > 0) {
            env_events.push_back({s - C, 1});
            env_events.push_back({s, -2});
            env_events.push_back({s + C, 1});
        }
    }
    
    // Sort Envelope Events to establish B_X(x)
    sort(env_events.begin(), env_events.end());
    
    vector<long long> unique_S = S;
    sort(unique_S.begin(), unique_S.end());
    unique_S.erase(unique(unique_S.begin(), unique_S.end()), unique_S.end());
    
    vector<pair<long long, long long>> candidate_X;
    long long cur_val_env = 0, cur_slope_env = 0;
    long long last_pos_env = env_events.empty() ? 0 : env_events[0].pos;
    int e_idx = 0;
    
    // Compute B_X(x) for all unique S candidates
    for (long long x : unique_S) {
        while (e_idx < env_events.size() && env_events[e_idx].pos <= x) {
            cur_val_env += cur_slope_env * (env_events[e_idx].pos - last_pos_env);
            last_pos_env = env_events[e_idx].pos;
            cur_slope_env += env_events[e_idx].delta_slope;
            e_idx++;
        }
        long long val_at_x = cur_val_env + cur_slope_env * (x - last_pos_env);
        candidate_X.push_back({val_at_x, x});
    }
    
    // Sort candidates descending by their absolute theoretical maximum potential!
    sort(candidate_X.rbegin(), candidate_X.rend());
    
    // Arrays allocating presorted relative order mappings
    vector<int> P_S(K), P_T(K), P_TS(K);
    for(int i = 0; i < K; i++) {
        P_S[i] = P_T[i] = P_TS[i] = i;
    }
    
    sort(P_S.begin(), P_S.end(), [&](int a, int b) { return S[a] < S[b]; });
    sort(P_T.begin(), P_T.end(), [&](int a, int b) { return T[a] < T[b]; });
    sort(P_TS.begin(), P_TS.end(), [&](int a, int b) { return (T[a] - S[a]) < (T[b] - S[b]); });
    
    long long best_improvement = 0;
    
    // Pre-allocated arrays for the inner loop (avoids continuous re-allocations)
    vector<Event> L1(K), L2(K), L3(K), L4(K);
    Event L0[1];
    
    for (auto cand : candidate_X) {
        // --- Branch and Bound Pruning ---
        // If the absolute best this 'x' could do is worse than what we already found,
        // it's mathematically impossible for *any* remaining candidate to beat it.
        if (cand.first <= best_improvement) {
            break; 
        }
        
        long long x = cand.second;
        
        int M = 0;
        int head1 = 0, head2 = 0, head3 = 0, head4 = 0;
        
        // Populate the 5 dynamically sorted event lists in absolute O(K) time
        for (int i : P_S) {
            if (S[i] <= x) continue;
            if (T[i] - 2 * S[i] + x - 1 > 0) {
                L1[head1++] = {2 * S[i] - x + 1, 1};
            }
        }
        
        for (int i : P_T) {
            bool g1 = (S[i] <= x && T[i] > x + 1);
            bool g2 = (S[i] > x && T[i] - 2 * S[i] + x - 1 > 0);
            if (g1) M++;
            if (g1 || g2) {
                L2[head2++] = {T[i], -2};
            }
            if (g1) {
                L3[head3++] = {2 * T[i] - x - 1, 1};
            }
        }
        
        for (int i : P_TS) {
            if (S[i] > x && T[i] - 2 * S[i] + x - 1 > 0) {
                L4[head4++] = {2 * T[i] - 2 * S[i] + x - 1, 1};
            }
        }
        
        int n0 = 0;
        if (M > 0) {
            L0[0] = {x + 1, M};
            n0 = 1;
        }
        
        // O(1) multi-pointer fast merge evaluate for variable `y`
        Event* lists[5] = {L0, L1.data(), L2.data(), L3.data(), L4.data()};
        int sizes[5] = {n0, head1, head2, head3, head4};
        int ptrs[5] = {0, 0, 0, 0, 0};
        
        long long cur_val = 0;
        long long cur_slope = 0;
        long long last_y = -1;
        long long current_x_best = 0;
        
        while (true) {
            long long min_val = 4e18; // Virtual Infinity
            for (int i = 0; i < 5; i++) {
                if (ptrs[i] < sizes[i]) {
                    if (lists[i][ptrs[i]].pos < min_val) {
                        min_val = lists[i][ptrs[i]].pos;
                    }
                }
            }
            if (min_val == 4e18) break;
            
            if (last_y != -1) {
                cur_val += cur_slope * (min_val - last_y);
            }
            
            // Check for peak during sweep mapping
            if (cur_val > current_x_best) {
                current_x_best = cur_val;
            }
            
            long long sum_w = 0;
            for (int i = 0; i < 5; i++) {
                while (ptrs[i] < sizes[i] && lists[i][ptrs[i]].pos == min_val) {
                    sum_w += lists[i][ptrs[i]].w;
                    ptrs[i]++;
                }
            }
            
            last_y = min_val;
            cur_slope += sum_w;
        }
        
        if (current_x_best > best_improvement) {
            best_improvement = current_x_best;
        }
    }
    
    // Global improvement is resolved against the absolute original route times
    cout << original_dist - best_improvement << "\n";
    
    return 0;
}