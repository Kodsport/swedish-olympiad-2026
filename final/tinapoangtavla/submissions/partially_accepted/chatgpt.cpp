// O(N * F * P log P)
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int INVALID = -1;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, P, H, F;
    if(!(cin >> N >> P >> H >> F)) return 0;

    struct Sub { int w, p; char r; };
    vector<vector<int>> wrongH(N, vector<int>(P, 0)); // wrongs during H (before freeze) per team/problem
    vector<vector<bool>> solvedH(N, vector<bool>(P, false));
    vector<int> base_pre_p(N,0);
    vector<ll> base_pre_t(N,0);

    for(int i=0;i<H;i++){
        int w,p; string r; cin >> w >> p >> r; --w; --p;
        if(solvedH[w][p]) continue;
        if(r == "WA") wrongH[w][p]++;
        else {
            solvedH[w][p] = true;
            base_pre_p[w] += 1;
            base_pre_t[w] += wrongH[w][p];
        }
    }

    vector<Sub> Fsubs(F);
    for(int i=0;i<F;i++){
        int w,p; string r; cin >> w >> p >> r; --w; --p;
        Fsubs[i] = {w,p,r=="AC"?'A':'W'};
    }

    // find last frozen submission position for each (team,problem) and count frozen per (team,problem)
    vector<vector<int>> lastIndexPos(N, vector<int>(P, INVALID));
    vector<vector<int>> frozen_counts(N, vector<int>(P,0));
    for(int idx=0; idx<F; ++idx){
        auto &s = Fsubs[idx];
        frozen_counts[s.w][s.p]++;
        lastIndexPos[s.w][s.p] = idx; // last occurrence remains
    }

    // delta per (team,problem) = wrongH + (#frozen before last)
    vector<vector<int>> delta(N, vector<int>(P,0));
    vector<vector<bool>> has_last(N, vector<bool>(P,false));
    for(int w=0; w<N; ++w){
        for(int p=0; p<P; ++p){
            if(lastIndexPos[w][p] != INVALID){
                has_last[w][p] = true;
                int beforeFrozen = frozen_counts[w][p] - 1;
                delta[w][p] = wrongH[w][p] + beforeFrozen;
            }
        }
    }

    // public view (what others know) starts as pre-freeze
    vector<int> public_p = base_pre_p;
    vector<ll> public_t = base_pre_t;
    // public unknown counts and sum deltas (what may still be revealed publicly)
    vector<int> public_u(N,0);
    vector<ll> public_sum_delta(N,0);
    for(int w=0; w<N; ++w){
        int cnt = 0; ll s = 0;
        for(int p=0;p<P;++p) if(has_last[w][p]){
            cnt++;
            s += delta[w][p];
        }
        public_u[w] = cnt;
        public_sum_delta[w] = s;
    }

    // private view per team (what that team personally knows immediately)
    vector<int> private_p = base_pre_p;
    vector<ll> private_t = base_pre_t;
    for(int w=0; w<N; ++w){
        for(int p=0;p<P;++p){
            if(has_last[w][p]){
                int li = lastIndexPos[w][p];
                char r = Fsubs[li].r; // team w knows this result
                if(r == 'A'){
                    private_p[w] += 1;
                    private_t[w] += delta[w][p];
                }
                // else W -> no change
            }
        }
    }

    // last index mapping for quick check during reveals
    vector<vector<int>> last_idx(N, vector<int>(P, INVALID));
    for(int w=0; w<N; ++w) for(int p=0;p<P;++p) if(lastIndexPos[w][p] != INVALID) last_idx[w][p] = lastIndexPos[w][p];

    vector<int> ans(N, -1);
    int remaining = N;

    // process current public state at step k
    auto process_step = [&](int k){
        int PMAX = P;
        vector<vector<ll>> lists_worst(PMAX+1), lists_best(PMAX+1);
        for(int w=0; w<N; ++w){
            int p_worst = public_p[w];
            ll t_worst = public_t[w];
            int p_best = p_worst + public_u[w];
            ll t_best = t_worst + public_sum_delta[w];
            lists_worst[p_worst].push_back(t_worst);
            lists_best[p_best].push_back(t_best);
        }
        for(int p=0;p<=PMAX;++p){
            sort(lists_worst[p].begin(), lists_worst[p].end());
            sort(lists_best[p].begin(), lists_best[p].end());
        }
        vector<int> suff_w(PMAX+2,0), suff_b(PMAX+2,0);
        for(int p=PMAX; p>=0; --p){
            suff_w[p] = suff_w[p+1] + (int)lists_worst[p].size();
            suff_b[p] = suff_b[p+1] + (int)lists_best[p].size();
        }

        for(int i=0;i<N;++i){
            if(ans[i] != -1) continue;
            // team i uses private known p/t
            int i_p = private_p[i];
            ll i_t = private_t[i];

            // min_count: how many teams j (public worst) are strictly better than i_known
            int larger_p = 0;
            if(i_p+1 <= PMAX) larger_p = suff_w[i_p+1];
            int same_p_less_t = (int)(lower_bound(lists_worst[i_p].begin(), lists_worst[i_p].end(), i_t) - lists_worst[i_p].begin());
            int count_min_incl = larger_p + same_p_less_t;

            // determine if i itself was counted in that (public worst could be strictly better than private known)
            bool self_counted_min = false;
            int i_pub_p = public_p[i];
            ll i_pub_t = public_t[i];
            if(i_pub_p > i_p) self_counted_min = true;
            else if(i_pub_p == i_p && i_pub_t < i_t) self_counted_min = true;
            int min_count = count_min_incl - (self_counted_min ? 1 : 0);

            // max_count: how many teams j (public best) can be strictly better than i_known
            int larger_p2 = 0;
            if(i_p+1 <= PMAX) larger_p2 = suff_b[i_p+1];
            int same_p2_less_t = (int)(lower_bound(lists_best[i_p].begin(), lists_best[i_p].end(), i_t) - lists_best[i_p].begin());
            int count_max_incl = larger_p2 + same_p2_less_t;

            // check self counted in public best
            bool self_counted_max = false;
            int i_pub_best_p = public_p[i] + public_u[i];
            ll i_pub_best_t = public_t[i] + public_sum_delta[i];
            if(i_pub_best_p > i_p) self_counted_max = true;
            else if(i_pub_best_p == i_p && i_pub_best_t < i_t) self_counted_max = true;
            int max_count = count_max_incl - (self_counted_max ? 1 : 0);

            if(min_count == max_count){
                ans[i] = k;
                remaining--;
            }
        }
    };

    // initial step (k=0)
    process_step(0);
    if(remaining > 0){
        for(int k=0; k<F; ++k){
            auto &s = Fsubs[k];
            int w = s.w, p = s.p;
            char r = s.r;
            // if this is the last frozen submission for (w,p), public view resolves it now
            if(last_idx[w][p] == k){
                // remove unknown from public
                public_u[w] -= 1;
                public_sum_delta[w] -= delta[w][p];
                if(r == 'A'){
                    public_p[w] += 1;
                    public_t[w] += delta[w][p];
                }
            } else {
                // revealing an earlier frozen submission (must be W), no effect on public model
            }
            process_step(k+1);
            if(remaining == 0) break;
        }
    }

    // print answers
    for(int i=0;i<N;++i){
        if(i) cout << ' ';
        if(ans[i] == -1) ans[i] = F;
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}
