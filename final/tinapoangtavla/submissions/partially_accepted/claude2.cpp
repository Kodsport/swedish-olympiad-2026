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
    // For each (team, problem): indices of freeze submissions
    vector<vector<vector<int>>> fidx(N+1, vector<vector<int>>(P+1));

    for(int i = 0; i < F; i++){
        cin >> fsubs[i].w >> fsubs[i].p >> fsubs[i].r;
        int w = fsubs[i].w, p = fsubs[i].p;
        if(!solved[w][p])
            fidx[w][p].push_back(i);
    }

    // For each (team, prob) with freeze subs: only last can be AC
    // penalty_if_solve[w][p] = wa_pre + (num_freeze_subs - 1)
    // last_reveal[w][p] = index of last freeze submission (0-indexed in reveal order)
    vector<vector<int>> pen_solve(N+1, vector<int>(P+1, 0));
    vector<vector<int>> last_rev(N+1, vector<int>(P+1, -1));

    for(int w = 1; w <= N; w++)
        for(int p = 1; p <= P; p++)
            if(!solved[w][p] && !fidx[w][p].empty()){
                pen_solve[w][p] = wa_pre[w][p] + (int)fidx[w][p].size() - 1;
                last_rev[w][p] = fidx[w][p].back();
            }

    // Compute each team's TRUE final result (teams know own submissions)
    vector<int> tpi(N+1, 0), tti(N+1, 0); // team's true (p, t)
    for(int w = 1; w <= N; w++)
        for(int p = 1; p <= P; p++){
            bool fin_solved = solved[w][p];
            if(!fin_solved && last_rev[w][p] >= 0 && fsubs[last_rev[w][p]].r == "AC")
                fin_solved = true;
            if(fin_solved){
                tpi[w]++;
                tti[w] += solved[w][p] ? wa_pre[w][p] : pen_solve[w][p];
            }
        }

    // For each team j, compute its uncertain problems and their resolve steps
    // Only the last freeze sub per (j, prob) matters for state change
    // Earlier subs are necessarily WA and reveal no info

    // For each team j, walk through state transitions and generate entries for 3 cases:
    // caseB[pi]: (end_step, team_j) - j contributes to caseB at level pi, drops at end_step
    // caseA[c]:  (ct, end_step, team_j) - j at caseA level c with penalty ct, exits at end_step
    // caseC[target]: (eff_pen, end_step, team_j) - j at caseC target with eff_pen, exits at end_step
    //
    // Team j uncertain w.r.t. team i (pi, ti):
    //   caseB: cp_j < pi AND cp_j + unc_j > pi
    //   caseA: cp_j = pi AND ct_j >= ti AND unc_j > 0
    //   caseC: cp_j + unc_j = pi AND cp_j < pi AND ct_j + total_unc_pen_j < ti

    vector<vector<pair<int,int>>> caseB(P+2);
    vector<vector<tuple<int,int,int>>> caseA(P+2), caseC(P+2);

    for(int j = 1; j <= N; j++){
        // Collect uncertain problems with their resolve step and result
        struct Ev { int step, pen; bool ac; };
        vector<Ev> evs;
        for(int p = 1; p <= P; p++){
            if(solved[j][p] || last_rev[j][p] < 0) continue;
            bool ac = (fsubs[last_rev[j][p]].r == "AC");
            evs.push_back({last_rev[j][p] + 1, pen_solve[j][p], ac}); // 1-indexed step
        }
        if(evs.empty()) continue;
        sort(evs.begin(), evs.end(), [](auto& a, auto& b){ return a.step < b.step; });

        int cp = 0, ct = 0;
        for(int p = 1; p <= P; p++) if(solved[j][p]){ cp++; ct += wa_pre[j][p]; }
        int unc = (int)evs.size();
        int tp = 0; for(auto& e : evs) tp += e.pen;

        // Track caseB bounds: pi in [lb, ub]
        int lb = cp + 1, ub = cp + unc - 1;
        // Track caseA: current level, ct
        // Track caseC: current target, eff_pen

        int cA_lev = cp, cA_ct = ct;
        int cC_tgt = cp + unc, cC_eff = ct + tp;

        for(int k = 0; k < (int)evs.size(); k++){
            int step = evs[k].step;
            bool ac = evs[k].ac;
            int pen = evs[k].pen;

            if(ac){
                // caseB: lb drops out
                if(lb <= ub && lb >= 0 && lb <= P)
                    caseB[lb].push_back({step, j});
                lb++;

                // caseA: exit current level
                if(cA_lev >= 0 && cA_lev <= P)
                    caseA[cA_lev].emplace_back(cA_ct, step, j);

                cp++; ct += pen; unc--; tp -= pen;
                cA_lev = cp; cA_ct = ct;

                // caseC: target stays (cp+unc same), eff stays
                if(unc == 0 && cC_tgt >= 0 && cC_tgt <= P && cC_tgt > cp)
                    caseC[cC_tgt].emplace_back(cC_eff, step, j);
                else if(unc == 0 && cC_tgt >= 0 && cC_tgt <= P)
                    {} // cp >= target, caseC doesn't apply (need cp < target)
                // if unc > 0, caseC continues
            } else { // WA
                // caseB: ub drops out
                if(lb <= ub && ub >= 0 && ub <= P)
                    caseB[ub].push_back({step, j});
                ub--;

                unc--; tp -= pen;

                // caseA: stays at same level. If unc=0, exit.
                if(unc == 0 && cA_lev >= 0 && cA_lev <= P)
                    caseA[cA_lev].emplace_back(cA_ct, step, j);

                // caseC: target decreases. Record old target exit.
                if(cC_tgt >= 0 && cC_tgt <= P && cC_tgt > cp)
                    caseC[cC_tgt].emplace_back(cC_eff, step, j);
                cC_tgt = cp + unc;
                cC_eff = ct + tp;
                // if unc=0, no new caseC entry (no uncertainty left)
            }
        }
    }

    // Now compute answers offline.
    // For each team i with (pi, ti):
    //   ansB = max end_step among caseB[pi] entries from teams != i
    //   ansA = max end_step among caseA[pi] entries with ct >= ti, from teams != i
    //   ansC = max end_step among caseC[pi] entries with eff_pen < ti, from teams != i
    //   answer[i] = max(ansB, ansA, ansC)

    // For caseB: per pi, compute top-2 (end_step, team)
    struct Top2 {
        pair<int,int> best = {-1,-1}, second = {-1,-1}; // (end_step, team)
        void add(int step, int team){
            if(step > best.first || (step == best.first && team != best.second)){
                if(team != best.second){ second = best; best = {step, team}; }
                else { best = {step, team}; }
            } else if((step > second.first || (step == second.first)) && team != best.second){
                second = {step, team};
            }
        }
        int query(int exclude_team){
            if(best.second != exclude_team) return best.first;
            return second.first;
        }
    };

    vector<Top2> caseBtop(P+2);
    for(int pi = 0; pi <= P; pi++)
        for(auto& [step, team] : caseB[pi])
            caseBtop[pi].add(step, team);

    // For caseA per level c: sort entries by ct descending, compute prefix top-2 of end_step
    // Query: max end_step among entries with ct >= ti, excluding team i
    // Need: sorted by ct desc, prefix top-2 (with team tracking for exclusion)
    struct PrefixTop2 {
        vector<int> cts; // sorted descending (for binary search)
        vector<pair<int,int>> best, second; // prefix top-2: (end_step, team)

        void build(vector<tuple<int,int,int>>& entries){
            // Sort by ct descending, then by end_step descending
            sort(entries.begin(), entries.end(), [](auto& a, auto& b){
                return get<0>(a) > get<0>(b) || (get<0>(a) == get<0>(b) && get<1>(a) > get<1>(b));
            });
            int n = entries.size();
            cts.resize(n);
            best.resize(n, {-1,-1});
            second.resize(n, {-1,-1});
            for(int i = 0; i < n; i++){
                auto [c, s, t] = entries[i];
                cts[i] = c;
                if(i == 0){
                    best[i] = {s, t};
                    second[i] = {-1, -1};
                } else {
                    best[i] = best[i-1];
                    second[i] = second[i-1];
                    // Try to insert (s, t)
                    if(s > best[i].first){
                        if(t != best[i].second) second[i] = best[i];
                        best[i] = {s, t};
                    } else if(t != best[i].second && s > second[i].first){
                        second[i] = {s, t};
                    }
                }
            }
        }

        int query(int ti, int exclude_team){
            if(cts.empty()) return -1;
            // Find rightmost index with ct >= ti
            // cts is sorted descending, so find last index where cts[idx] >= ti
            // = upper_bound for (ti) in reverse... use lower_bound on negated?
            // Just binary search: find largest idx such that cts[idx] >= ti
            int lo = 0, hi = (int)cts.size() - 1, pos = -1;
            while(lo <= hi){
                int mid = (lo + hi) / 2;
                if(cts[mid] >= ti){ pos = mid; lo = mid + 1; }
                else hi = mid - 1;
            }
            if(pos < 0) return -1;
            if(best[pos].second != exclude_team) return best[pos].first;
            return second[pos].first;
        }
    };

    vector<PrefixTop2> caseAtop(P+2);
    for(int c = 0; c <= P; c++)
        caseAtop[c].build(caseA[c]);

    // For caseC per target: sort entries by eff_pen ascending, compute prefix top-2
    // Query: max end_step among entries with eff_pen < ti, excluding team i
    struct PrefixTop2C {
        vector<int> eps; // eff_pen, sorted ascending
        vector<pair<int,int>> best, second;

        void build(vector<tuple<int,int,int>>& entries){
            sort(entries.begin(), entries.end(), [](auto& a, auto& b){
                return get<0>(a) < get<0>(b) || (get<0>(a) == get<0>(b) && get<1>(a) > get<1>(b));
            });
            int n = entries.size();
            eps.resize(n);
            best.resize(n, {-1,-1});
            second.resize(n, {-1,-1});
            for(int i = 0; i < n; i++){
                auto [e, s, t] = entries[i];
                eps[i] = e;
                if(i == 0){
                    best[i] = {s, t};
                    second[i] = {-1, -1};
                } else {
                    best[i] = best[i-1];
                    second[i] = second[i-1];
                    if(s > best[i].first){
                        if(t != best[i].second) second[i] = best[i];
                        best[i] = {s, t};
                    } else if(t != best[i].second && s > second[i].first){
                        second[i] = {s, t};
                    }
                }
            }
        }

        int query(int ti, int exclude_team){
            if(eps.empty()) return -1;
            // Find rightmost index with eff_pen < ti
            // eps sorted ascending: find last index where eps[idx] < ti
            int lo = 0, hi = (int)eps.size() - 1, pos = -1;
            while(lo <= hi){
                int mid = (lo + hi) / 2;
                if(eps[mid] < ti){ pos = mid; lo = mid + 1; }
                else hi = mid - 1;
            }
            if(pos < 0) return -1;
            if(best[pos].second != exclude_team) return best[pos].first;
            return second[pos].first;
        }
    };

    vector<PrefixTop2C> caseCtop(P+2);
    for(int t = 0; t <= P; t++)
        caseCtop[t].build(caseC[t]);

    // Compute answer for each team
    for(int i = 1; i <= N; i++){
        int pi = tpi[i], ti = tti[i];
        int ans = -1;

        // Case B
        int b = caseBtop[pi].query(i);
        if(b > ans) ans = b;

        // Case A
        if(pi <= P){
            int a = caseAtop[pi].query(ti, i);
            if(a > ans) ans = a;
        }

        // Case C
        if(pi <= P){
            int c = caseCtop[pi].query(ti, i);
            if(c > ans) ans = c;
        }

        cout << max(0, ans);
        if(i < N) cout << ' ';
    }
    cout << '\n';

    return 0;
}
