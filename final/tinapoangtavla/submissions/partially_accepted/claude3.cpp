#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, P, H, F;
    cin >> N >> P >> H >> F;

    vector<vector<bool>> solved(N+1, vector<bool>(P+1, false));
    vector<vector<int>> wa_pre(N+1, vector<int>(P+1, 0));

    for(int i = 0; i < H; i++){
        int w, p; string r;
        cin >> w >> p >> r;
        if(solved[w][p]) continue;
        if(r == "AC") solved[w][p] = true;
        else wa_pre[w][p]++;
    }

    struct FS { int w, p; string r; };
    vector<FS> fsubs(F);
    vector<vector<vector<int>>> fidx(N+1, vector<vector<int>>(P+1));

    for(int i = 0; i < F; i++){
        cin >> fsubs[i].w >> fsubs[i].p >> fsubs[i].r;
        int w = fsubs[i].w, p = fsubs[i].p;
        if(!solved[w][p])
            fidx[w][p].push_back(i);
    }

    vector<vector<int>> pen_solve(N+1, vector<int>(P+1, 0));
    vector<vector<int>> last_rev(N+1, vector<int>(P+1, -1));

    for(int w = 1; w <= N; w++)
        for(int p = 1; p <= P; p++)
            if(!solved[w][p] && !fidx[w][p].empty()){
                pen_solve[w][p] = wa_pre[w][p] + (int)fidx[w][p].size() - 1;
                last_rev[w][p] = fidx[w][p].back();
            }

    // Each team's TRUE final result (teams know own submissions)
    vector<int> tpi(N+1, 0), tti(N+1, 0);
    for(int w = 1; w <= N; w++)
        for(int p = 1; p <= P; p++){
            bool fin = solved[w][p];
            if(!fin && last_rev[w][p] >= 0 && fsubs[last_rev[w][p]].r == "AC")
                fin = true;
            if(fin){
                tpi[w]++;
                tti[w] += solved[w][p] ? wa_pre[w][p] : pen_solve[w][p];
            }
        }

    // Generate offline entries for cases A, B, C
    // caseB[pi]: j uncertain for any (pi, ti) when cp_j < pi < cp_j + unc_j
    // caseA[c]:  j uncertain for (pi=c, ti<=ct_j) when cp_j=c, unc_j>0
    // caseC[tgt]: j uncertain for (pi=tgt, ti>eff_pen) when cp_j+unc_j=tgt, cp_j<tgt, unc_j>0

    vector<vector<pair<int,int>>> caseBe(P+2);           // [pi] -> (end_step, team)
    vector<vector<tuple<int,int,int>>> caseAe(P+2);      // [c]  -> (ct, end_step, team)
    vector<vector<tuple<int,int,int>>> caseCe(P+2);      // [tgt]-> (eff_pen, end_step, team)

    for(int j = 1; j <= N; j++){
        struct Ev { int step, pen; bool ac; };
        vector<Ev> evs;
        for(int p = 1; p <= P; p++){
            if(solved[j][p] || last_rev[j][p] < 0) continue;
            evs.push_back({last_rev[j][p] + 1, pen_solve[j][p],
                           fsubs[last_rev[j][p]].r == "AC"});
        }
        if(evs.empty()) continue;
        sort(evs.begin(), evs.end(), [](auto& a, auto& b){ return a.step < b.step; });

        int cp = 0, ct = 0;
        for(int p = 1; p <= P; p++) if(solved[j][p]){ cp++; ct += wa_pre[j][p]; }
        int unc = (int)evs.size();
        int tp = 0; for(auto& e : evs) tp += e.pen;

        int lb = cp + 1, ub = cp + unc - 1;
        int cA_lev = cp, cA_ct = ct;
        int cC_tgt = cp + unc, cC_eff = ct + tp;

        for(int k = 0; k < (int)evs.size(); k++){
            int step = evs[k].step;
            bool ac = evs[k].ac;
            int pen = evs[k].pen;

            if(ac){
                // caseB: lower bound drops
                if(lb <= ub && lb >= 0 && lb <= P)
                    caseBe[lb].push_back({step, j});
                lb++;

                // caseA: exit current level
                if(cA_lev >= 0 && cA_lev <= P)
                    caseAe[cA_lev].emplace_back(cA_ct, step, j);

                // caseC: if unc becomes 0, record exit BEFORE updating cp
                if(unc - 1 == 0 && cC_tgt >= 0 && cC_tgt <= P && cC_tgt > cp)
                    caseCe[cC_tgt].emplace_back(cC_eff, step, j);

                cp++; ct += pen; unc--; tp -= pen;
                cA_lev = cp; cA_ct = ct;
                // caseC target & eff unchanged (cp+unc and ct+tp invariant under AC)
            } else {
                // caseB: upper bound drops
                if(lb <= ub && ub >= 0 && ub <= P)
                    caseBe[ub].push_back({step, j});
                ub--;

                // caseC: target changes, record old target exit BEFORE update
                if(cC_tgt >= 0 && cC_tgt <= P && cC_tgt > cp)
                    caseCe[cC_tgt].emplace_back(cC_eff, step, j);

                unc--; tp -= pen;

                // caseA: if unc becomes 0, exit
                if(unc == 0 && cA_lev >= 0 && cA_lev <= P)
                    caseAe[cA_lev].emplace_back(cA_ct, step, j);

                // Update caseC to new target
                cC_tgt = cp + unc;
                cC_eff = ct + tp;
            }
        }
    }

    // Build query structures

    // caseB: per pi, top-2 (end_step, team) from different teams
    struct Top2 {
        pair<int,int> b = {-1,-1}, s = {-1,-1};
        void add(int step, int team){
            if(step > b.first){
                if(team != b.second) s = b;
                b = {step, team};
            } else if(team != b.second && step > s.first){
                s = {step, team};
            }
        }
        int query(int ex){ return b.second != ex ? b.first : s.first; }
    };

    vector<Top2> Btop(P+2);
    for(int pi = 0; pi <= P; pi++)
        for(auto& [st, tm] : caseBe[pi])
            Btop[pi].add(st, tm);

    // caseA[c]: sorted by ct desc, prefix top-2 of end_step
    // Query: max end_step where ct >= ti, excluding team i
    struct PfxTop2 {
        vector<int> keys;
        vector<pair<int,int>> b, s;
        void build(vector<tuple<int,int,int>>& E){
            sort(E.begin(), E.end(), [](auto& a, auto& b){
                return get<0>(a) > get<0>(b);
            });
            int n = E.size();
            keys.resize(n); b.resize(n); s.resize(n);
            for(int i = 0; i < n; i++){
                auto [c, st, tm] = E[i];
                keys[i] = c;
                if(i == 0){ b[i] = {st, tm}; s[i] = {-1, -1}; }
                else {
                    b[i] = b[i-1]; s[i] = s[i-1];
                    if(st > b[i].first){
                        if(tm != b[i].second) s[i] = b[i];
                        b[i] = {st, tm};
                    } else if(tm != b[i].second && st > s[i].first){
                        s[i] = {st, tm};
                    }
                }
            }
        }
        int query(int thresh, int ex){
            if(keys.empty()) return -1;
            // rightmost index with key >= thresh (keys sorted desc)
            int lo = 0, hi = (int)keys.size()-1, pos = -1;
            while(lo <= hi){
                int mid = (lo+hi)/2;
                if(keys[mid] >= thresh){ pos = mid; lo = mid+1; }
                else hi = mid-1;
            }
            if(pos < 0) return -1;
            return b[pos].second != ex ? b[pos].first : s[pos].first;
        }
    };

    vector<PfxTop2> Atop(P+2);
    for(int c = 0; c <= P; c++) Atop[c].build(caseAe[c]);

    // caseC[tgt]: sorted by eff_pen asc, prefix top-2 of end_step
    // Query: max end_step where eff_pen < ti, excluding team i
    struct PfxTop2C {
        vector<int> keys;
        vector<pair<int,int>> b, s;
        void build(vector<tuple<int,int,int>>& E){
            sort(E.begin(), E.end(), [](auto& a, auto& b){
                return get<0>(a) < get<0>(b);
            });
            int n = E.size();
            keys.resize(n); b.resize(n); s.resize(n);
            for(int i = 0; i < n; i++){
                auto [ep, st, tm] = E[i];
                keys[i] = ep;
                if(i == 0){ b[i] = {st, tm}; s[i] = {-1, -1}; }
                else {
                    b[i] = b[i-1]; s[i] = s[i-1];
                    if(st > b[i].first){
                        if(tm != b[i].second) s[i] = b[i];
                        b[i] = {st, tm};
                    } else if(tm != b[i].second && st > s[i].first){
                        s[i] = {st, tm};
                    }
                }
            }
        }
        int query(int thresh, int ex){
            if(keys.empty()) return -1;
            // rightmost index with key < thresh (keys sorted asc)
            int lo = 0, hi = (int)keys.size()-1, pos = -1;
            while(lo <= hi){
                int mid = (lo+hi)/2;
                if(keys[mid] < thresh){ pos = mid; lo = mid+1; }
                else hi = mid-1;
            }
            if(pos < 0) return -1;
            return b[pos].second != ex ? b[pos].first : s[pos].first;
        }
    };

    vector<PfxTop2C> Ctop(P+2);
    for(int t = 0; t <= P; t++) Ctop[t].build(caseCe[t]);

    // Answer each team
    for(int i = 1; i <= N; i++){
        int pi = tpi[i], ti = tti[i];
        int ans = -1;
        if(pi <= P){
            ans = max(ans, Btop[pi].query(i));
            ans = max(ans, Atop[pi].query(ti, i));
            ans = max(ans, Ctop[pi].query(ti, i));
        }
        cout << max(0, ans);
        if(i < N) cout << ' ';
    }
    cout << '\n';
}
