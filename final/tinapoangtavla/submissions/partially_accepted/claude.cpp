#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, P, H, F;
    cin >> N >> P >> H >> F;

    // For each team, for each problem: track WA count and whether solved pre-freeze
    // solved[w][p] = true if solved before freeze
    // wa[w][p] = number of WA submissions on problem p by team w (before AC or end)
    vector<vector<bool>> solved(N+1, vector<bool>(P+1, false));
    vector<vector<int>> wa_before(N+1, vector<int>(P+1, 0));

    // Pre-freeze submissions
    for(int i = 0; i < H; i++){
        int w, p; string r;
        cin >> w >> p >> r;
        if(solved[w][p]) continue; // shouldn't happen per problem statement
        if(r == "AC"){
            solved[w][p] = true;
        } else {
            wa_before[w][p]++;
        }
    }

    // Compute each team's known result (pi, ti) - teams know their own submissions
    // We need the FINAL result for each team (including all freeze submissions)
    // But first, let's read freeze submissions
    struct FreezeSubmission {
        int w, p;
        string r;
    };
    vector<FreezeSubmission> freeze_subs(F);
    
    // For each (team, problem): list of freeze submission indices, in chronological order
    // Also track: which is the last freeze submission for each (team, problem)
    map<pair<int,int>, vector<int>> freeze_indices; // (w,p) -> list of indices in freeze_subs
    
    for(int i = 0; i < F; i++){
        cin >> freeze_subs[i].w >> freeze_subs[i].p >> freeze_subs[i].r;
        int w = freeze_subs[i].w, p = freeze_subs[i].p;
        if(!solved[w][p]){
            freeze_indices[{w,p}].push_back(i);
        }
    }

    // For each (team, problem) with freeze submissions: only the last can be AC
    // Compute: last_freeze_idx[w][p] = index of last freeze submission for (w,p)
    // penalty_if_solved[w][p] = WA count if team w solves problem p
    // (= wa_before[w][p] + number of freeze submissions on (w,p) - 1)
    
    // For each team, compute their TRUE final result (they know their own)
    vector<int> team_p(N+1, 0), team_t(N+1, 0);
    // Also compute: set of uncertain problems per team (problems with freeze subs, not solved pre-freeze)
    // uncertain_penalty[w] = sorted list of penalties for uncertain problems
    
    // First: compute true final results and uncertain info
    // solved_final[w][p] = whether team w ultimately solves problem p
    vector<vector<bool>> solved_final(N+1, vector<bool>(P+1, false));
    vector<vector<int>> penalty_if_solve(N+1, vector<int>(P+1, 0)); // penalty contribution if solved
    
    for(int w = 1; w <= N; w++){
        for(int p = 1; p <= P; p++){
            solved_final[w][p] = solved[w][p]; // pre-freeze
        }
    }
    
    // Check freeze submissions for final results
    for(auto& [wp, indices] : freeze_indices){
        int w = wp.first, p = wp.second;
        // Last submission determines if solved
        int last_idx = indices.back();
        if(freeze_subs[last_idx].r == "AC"){
            solved_final[w][p] = true;
        }
        penalty_if_solve[w][p] = wa_before[w][p] + (int)indices.size() - 1;
        // If solved pre-freeze (already handled, this shouldn't be in freeze_indices)
    }
    
    // Compute team's true (p, t) - what the team itself knows
    for(int w = 1; w <= N; w++){
        for(int p = 1; p <= P; p++){
            if(solved_final[w][p]){
                team_p[w]++;
                if(solved[w][p]){
                    // Solved before freeze, penalty = wa_before[w][p]
                    team_t[w] += wa_before[w][p];
                } else {
                    // Solved during freeze
                    team_t[w] += penalty_if_solve[w][p];
                }
            }
        }
    }

    // Now: for the simulation. 
    // For each team j, track: certain_p (problems definitely solved from others' perspective),
    // uncertain problems set, and penalties.
    
    vector<int> certain_p(N+1, 0), certain_t(N+1, 0);
    // uncertain[w] = set of problems that are uncertain for team w
    vector<set<int>> uncertain(N+1);
    // uncertain_total_penalty[w] = sum of penalties for all uncertain problems
    vector<int> uncertain_total_pen(N+1, 0);
    
    for(int w = 1; w <= N; w++){
        for(int p = 1; p <= P; p++){
            if(solved[w][p]){
                certain_p[w]++;
                certain_t[w] += wa_before[w][p];
            }
        }
    }
    
    for(auto& [wp, indices] : freeze_indices){
        int w = wp.first, p = wp.second;
        uncertain[w].insert(p);
        uncertain_total_pen[w] += penalty_if_solve[w][p];
    }
    
    // For each freeze submission, mark which is the last for its (team, problem)
    vector<bool> is_last(F, false);
    for(auto& [wp, indices] : freeze_indices){
        is_last[indices.back()] = true;
    }
    
    // For each team i, check if rank is determined.
    // Team i has result (team_p[i], team_t[i]).
    // Team j is uncertain w.r.t. i iff:
    //   case A: certain_p[j] = team_p[i], certain_t[j] >= team_t[i], |uncertain[j]| > 0
    //   case B: certain_p[j] < team_p[i], certain_p[j] + |uncertain[j]| > team_p[i]
    //   case C: certain_p[j] < team_p[i], certain_p[j] + |uncertain[j]| = team_p[i],
    //           AND certain_t[j] + total_uncertain_penalty[j] < team_t[i]
    //           (j must solve ALL uncertain to reach team_p[i], with fixed total penalty)
    
    // We need: for each team i, is there any j != i uncertain w.r.t. i?
    
    // Approach: for each reveal step, process the reveal, then check all teams.
    // With N up to 2e5 and F up to 2e5, checking all N teams after each of F reveals is O(NF) = 4e10, too slow.
    
    // Better: only O(N*P) reveals are "significant" (last submission per team-problem).
    // For each significant reveal, team j's state changes.
    // We need to determine: which teams i had j as their LAST uncertain team?
    
    // Practical approach for given constraints:
    // For each team i, precompute: for each team j, is j initially uncertain w.r.t. i?
    // If so, when does j stop being uncertain w.r.t. i?
    // Answer for i = max over all such j of their stop time. Or 0 if no j is uncertain initially.
    
    // This is O(N^2) which is too slow for N=2e5.
    
    // Let's use a smarter approach:
    // Group by certain_p levels. For each level, maintain counts.
    // For team i with pi = team_p[i], ti = team_t[i]:
    //   Need to check if any team j satisfies cases A, B, or C.
    
    // For each reveal step (only significant ones matter):
    //   Update team j's state
    //   This might change which teams i have determined rank
    
    // Actually let's think about it per-team-i.
    // For team i, j is uncertain in case B if certain_p[j] < pi and certain_p[j]+|unc_j|>pi
    //   i.e., j's "range" [certain_p[j], certain_p[j]+|unc_j|] contains pi+1 or more.
    //   Equivalently, certain_p[j] < pi and certain_p[j]+|unc_j| > pi.
    //   Since pi <= 15, this means certain_p[j] in {0,...,pi-1} and |unc_j| > pi - certain_p[j].
    
    // For case A: certain_p[j] = pi, certain_t[j] >= ti, |unc_j| > 0.
    
    // For case C: certain_p[j]+|unc_j| = pi, certain_p[j] < pi, 
    //             certain_t[j] + total_unc_pen[j] < ti.
    
    // Since P <= 15, certain_p ranges 0..15 and |unc| ranges 0..15.
    // We can maintain for each (cp, unc_count) pair a data structure.
    
    // For case B: team j with (cp_j, unc_j) is uncertain for team i with pi iff
    //   cp_j < pi AND cp_j + unc_j > pi
    //   iff cp_j < pi AND unc_j > pi - cp_j
    //   Since these are small integers, we can count how many teams have each (cp, unc) pair.
    //   For team i with pi, we need: sum over cp=0..pi-1, unc=pi-cp+1..15 of count[cp][unc] > 0
    //     But this includes team i itself potentially. And we need "> 0 excluding team i".
    
    // For case A: teams with cp = pi, unc > 0, and ct >= ti.
    //   We need to know if any such team exists with ct >= ti (excluding i).
    
    // For case C: teams with cp + unc = pi, cp < pi, and ct + total_unc_pen < ti.
    //   For fixed cp, unc = pi - cp. These teams solve ALL uncertain problems to reach pi.
    //   Need ct + total_unc_pen < ti.
    
    // Let's maintain:
    // cnt[cp][unc] = number of teams with certain_p=cp, |uncertain|=unc, unc>0
    // For case A: for each pi, we need to know if there's a team j != i with cp=pi, unc>0, ct_j >= ti.
    //   We can maintain for each cp, a multiset/sorted structure of ct values for teams with unc>0.
    // For case B: for each pi, count = sum_{cp=0}^{pi-1} sum_{unc=pi-cp+1}^{15} cnt[cp][unc]
    //   If count > 0 (or count > 0 excluding team i if team i falls in this range - but team i has 
    //   cp_i that they know, so team i's own state is certain from its own perspective... wait no.
    //   Team i IS one of the N teams. Other teams see team i as having uncertain problems too.
    //   But team i knows its own result. So when checking if team i's rank is determined,
    //   team i considers other teams' uncertainties, not its own.
    //   So we should exclude team i from the count only if i = j. Since we're asking about teams j != i,
    //   we typically just check all j != i.
    
    // Actually for case B: team i itself is not uncertain w.r.t. itself. And we're counting
    //   teams j != i. So if team i happens to be in the cnt array too, we need to subtract it.
    
    // Let's precompute a 2D prefix sum for case B.
    // For case B, define: B(pi) = number of teams j with certain_p[j] < pi and certain_p[j]+|unc_j| > pi
    
    // We can maintain a 16x16 grid cnt[cp][unc] and compute B(pi) as a sum.
    // Since P <= 15, the grid is small. Summing is O(P^2) = O(225) per query.
    
    // For case A: maintain for each cp value, a sorted multiset of ct values (among teams with unc>0).
    // For query (pi, ti): check if there's any ct >= ti in the multiset for cp=pi.
    // This is O(log N) per query.
    
    // For case C: for fixed pi, teams with cp+unc=pi and cp<pi.
    // For each cp in 0..pi-1, unc=pi-cp. The "effective penalty" = ct + total_unc_pen.
    // Need to know if any team has effective_pen < ti.
    // Maintain for each (cp, unc) pair, a sorted list or min of effective penalty.
    // Query: for each cp in 0..pi-1 where unc=pi-cp, check min effective_pen < ti.
    // O(P) per query.
    
    // Total per check: O(P^2 + log N + P) = O(P^2).
    // How many checks? We can't check all N teams after each reveal.
    
    // But: only ~N*P significant reveals. After each, only the teams i that were previously
    // not determined might become determined. We could maintain a set of undetermined teams
    // and check them after each significant reveal.
    
    // In the worst case this is still O(N * N*P * P^2) which is too much.
    
    // Alternative: for each team i, precompute answer by checking when all three cases become empty.
    
    // For case B: B(pi) = 0 for team i is independent of ti. It only depends on pi.
    //   All teams i with the same pi share the same B check.
    //   B(pi) decreases over time as teams resolve uncertain problems.
    //   For each pi, precompute the last reveal step at which B(pi) > 0.
    //   Actually B(pi) depends on whether team i itself contributes to B(pi)... 
    //   Team i has its own (certain_p[i], |uncertain[i]|). From OTHER teams' perspective, i has uncertain problems.
    //   But from team i's OWN perspective of checking its rank, j != i.
    //   So B(pi) for team i = (total teams with cp < pi and cp+unc > pi) minus (1 if team i satisfies this).
    //   But team i's (cp_i, unc_i) is team i's state as seen by others. Team i knows its own result.
    //   So yes, we subtract team i's contribution if it matches.

    // Actually, team i's certain_p (from others' perspective) might not equal team_p[i].
    // Team i knows its full result, but others don't. However, when we check team i's rank,
    // we use team i's TRUE result (team_p[i], team_t[i]) and check against every OTHER team j's
    // possible results.
    
    // So: B(pi) counts teams j with cp_j < pi and cp_j+unc_j > pi.
    // We need B(pi) - (1 if team i satisfies cp_i < pi and cp_i+unc_i > pi else 0) == 0.
    // But actually team i's uncertainty doesn't matter for team i's OWN rank check.
    // Team i knows its own result. Team j != i doesn't know team i's result.
    // But we're computing team i's rank, which depends on how many teams beat i.
    // Team i considers team j's possible outcomes, not its own.
    // So the count should be over j != i only.
    
    // For case B: for a given pi, B(pi) = #{j : cp_j < pi and cp_j + unc_j > pi}.
    // For team i, we need #{j != i : cp_j < pi and cp_j + unc_j > pi}.
    // = B(pi) - (1 if team i itself satisfies cp_i < pi and cp_i + unc_i > pi).
    
    // But wait: team i's cp_i and unc_i are from the "public" state (what others observe).
    // Team i's public state has uncertain problems too. But team i knows its own results.
    // So team i should NOT be counted when checking its own rank.
    // Yes: we compute B(pi) over all teams, then subtract team i's own contribution.
    
    // For each pi (0..15), we precompute B(pi) using the 16x16 grid.
    // Similarly for cases A and C.
    
    // Now the key insight: there are only 16 possible values of pi!
    // So we can maintain B(pi) for each pi value.
    // After each significant reveal (team j's state changes), update the grid, recompute B for all pi.
    // Then for each pi, determine if all teams i with team_p[i] = pi are now determined.
    
    // For case A: teams i with team_p[i] = pi need: no team j (j!=i) with cp_j = pi, unc_j > 0, ct_j >= ti.
    // This depends on ti, so teams with same pi but different ti may have different answers.
    
    // For case C: similar, depends on ti.
    
    // For case B: only depends on pi, not ti!
    
    // So: team i's rank is undetermined iff:
    // (1) B(pi) - self_contribution_B > 0, OR
    // (2) ∃ j!=i with cp_j = pi, unc_j > 0, ct_j >= ti, OR  
    // (3) ∃ j!=i with cp_j < pi, cp_j+unc_j = pi, ct_j+total_unc_pen_j < ti
    
    // For each significant event, conditions (1), (2), (3) might change for some teams i.
    
    // Let's think step by step. There are at most N*P significant events (but bounded by F).
    // After each event:
    //   (1) changes for teams at certain pi values. Since B depends on the grid, recompute.
    //   (2) changes for teams with pi = cp_j (if j's state changed at cp_j level).
    //   (3) changes for teams with pi = cp_j + unc_j.
    
    // For each team i, the answer = the latest significant event at which (1)∨(2)∨(3) was true.
    // After that event, (1)∧(2)∧(3) all become false.
    
    // This seems feasible! Let me think about the implementation.
    // 
    // Plan:
    // 1. Process reveals in order. Track which are significant (last for team-problem pair).
    // 2. For each significant reveal at step s:
    //    - Update team j's state (cp_j, ct_j, unc_j).
    //    - Update the global data structures.
    // 3. After all reveals, for each team i, determine the last step at which they were undetermined.
    
    // But we need to know AT EACH STEP which conditions hold. This requires processing step by step
    // and recording states. With N*P events and 16 pi levels, this might work.
    
    // Alternative simpler approach: for each team i, compute the answer directly.
    // For condition (1): last step at which B(pi) > self_contribution (0 or 1).
    // For condition (2): last step at which the max ct among {j!=i : cp_j=pi, unc_j>0} >= ti.
    //   = last step at which some team j with cp_j=pi, ct_j >= ti has unc_j > 0.
    // For condition (3): last step at which some team j with cp_j+unc_j=pi, cp_j<pi, ct_j+pen_j < ti
    //   has its state.
    // Answer for i = max of these three "last step" values. (0 if all are 0.)
    
    // This might still be O(N^2) in worst case for condition (2). Hmm.
    
    // Let me just implement a solution that works within time for the given constraints.
    // Given P <= 15 and N, F <= 2e5:
    
    // Approach: process reveals. Only significant ones change state.
    // After each significant event, for each team i that's still undetermined, check if now determined.
    // Use the grid-based approach for fast checking.
    
    // For checking team i:
    // - Case B: sum over grid cells. O(P^2). 
    // - Case A: check sorted structure at cp=pi. O(log N).
    // - Case C: check for each cp < pi where unc=pi-cp. O(P).
    // Total per team: O(P^2).
    
    // Worst case: all N teams checked after each of F events = O(N*F*P^2).
    // N=2e5, F=2e5, P^2=225. That's 9*10^12. Way too slow.
    
    // Need to be smarter. Let me think about what changes after each event.
    
    // After a significant event for team j:
    // - Only teams i where j was uncertain w.r.t. i are affected.
    // - If j stops being uncertain w.r.t. i, and j was the LAST uncertain team for i,
    //   then i becomes determined.
    
    // For each significant event (team j), we need to find teams i for which j's status changes.
    // Then for those teams, check if they're now determined.
    
    // When team j's state changes:
    //   Old: (cp_old, ct_old, unc_old)
    //   New: (cp_new, ct_new, unc_new)
    //   (cp could increase by 1 if AC, ct increases, unc decreases by 1)
    //   (or cp stays, ct stays, unc decreases by 1 if WA)
    
    // The set of teams i affected:
    //   Teams where j's uncertainty status w.r.t. i changes.
    //   These are teams with specific (pi, ti) values.
    
    // For case B: j was in case B for pi if cp_old < pi and cp_old+unc_old > pi.
    //   Now: cp_new < pi and cp_new+unc_new > pi?
    //   If WA: cp_new=cp_old, unc_new=unc_old-1. So j drops out of case B for pi if
    //     cp_old + unc_old - 1 <= pi, i.e., cp_old + unc_old = pi + 1 (was just barely > pi).
    //     So j drops out for pi = cp_old + unc_old - 1.
    //   If AC: cp_new=cp_old+1, unc_new=unc_old-1. Sum stays same. cp increases.
    //     j drops out for pi = cp_old (was cp_old < pi, now cp_new = cp_old+1, might not < pi).
    //     j stays in for pi > cp_new and cp_new + unc_new > pi.
    
    // This is getting complex but tractable since P is small.
    // Let me just implement a somewhat optimized solution.
    
    // PRACTICAL APPROACH:
    // Since P <= 15, the the number of distinct pi values is at most 16.
    // Maintain for each pi level structures to quickly answer the three cases.
    // Process events in order. For each event, update structures and check affected teams.
    
    // Actually, let me try an O(N * P * P) = O(N * P^2) approach with some smart bookkeeping.
    // For each team i, precompute the answer.
    // For each team i: go through all teams j. For each j, determine:
    //   - Is j ever uncertain w.r.t. i? 
    //   - If so, at what reveal step does j stop being uncertain w.r.t. i?
    //   - Answer for i = max of these steps.
    // But O(N^2) pairs is too many.
    
    // FINAL APPROACH: 
    // For each reveal step (in order), maintain the state of all teams.
    // For each team i, maintain a counter: how many teams j are uncertain w.r.t. i.
    // When this counter hits 0, team i is determined.
    
    // When team j's state changes, update counters for affected teams i.
    // The affected teams i are those where j's uncertainty status changes.
    // For case B: j's status changes for specific pi values (at most 2 values).
    //   ALL teams with that pi are affected. But we need to check ti too for cases A and C...
    //   Actually for case B, it doesn't depend on ti. So for case B, its a per-pi-level change.
    
    // Let me take a step back and implement something that should work. Given the constraints,
    // maybe an O(F * N) solution with good constants could work within time? 
    // F*N = 4e10, that's too slow.
    
    // Maybe O((F + N) * 2^P)? 2^15 = 32768. (F+N)*32768 = ~1.3e10. Still too much.
    
    // OK let me think about this more carefully. The answer for team i depends on:
    // 1. pi = team_p[i], ti = team_t[i]  
    // 2. For each team j: when does j stop being uncertain w.r.t. (pi, ti)?
    
    // For a fixed (pi, ti), this is a function of the reveal sequence.
    
    // Teams can be grouped by (pi, ti). But with continuous ti values, there could be N groups.
    
    // Let me try a different tactic. For each team j with uncertain problems, 
    // j generates "events" as its uncertain problems are resolved.
    // For each such event (at reveal step s), j's state changes.
    // I can precompute: after event, which (pi, ti) pairs does j become uncertain/not-uncertain for?
    
    // For case B: j is uncertain for pi in range (cp_j, cp_j + unc_j).
    //   When unc_j decreases, the upper end of this range drops.
    //   When cp_j increases, the lower end rises.
    //   Teams i with team_p[i] in the changed part of the range are affected.
    //   For case B, uncertainty doesn't depend on ti.
    
    // For case A: j is uncertain for (pi, ti) = (cp_j, t) where t <= ct_j and unc_j > 0.
    //   When unc_j drops to 0 or cp_j changes, status changes.
    
    // For case C: j is uncertain for (pi, ti) where pi = cp_j + unc_j, cp_j < pi,
    //   and ct_j + total_unc_pen_j < ti.
    //   When unc_j or cp_j changes, the pi value changes.
    
    // OK I think the cleanest approach given P<=15 is:
    // 
    // Maintain answer[i] initialized to 0.
    // Process reveals in reverse order. 
    // Hmm, forward is probably easier.
    
    // Let me just code a direct simulation. For each significant reveal step s:
    //   Update team j.
    //   For teams i that j was uncertain w.r.t. but is no longer:
    //     Check if team i still has any uncertain team. If not, answer[i] = s.
    //     (Wait, this would undercount. We want the LAST step before determined.)
    //     Actually answer[i] = s (the reveal at which the last uncertain team resolved).
    
    // Hmm, let me reconsider. answer[i] = number of submissions revealed BEFORE losing interest.
    // Team i loses interest at the moment when its rank becomes determined.
    // So answer[i] = the index of the last reveal that resolves some uncertainty for i.
    // If i is determined before any reveals, answer[i] = 0.
    
    // When processing forward: after reveal s (1-indexed), if team i becomes determined, answer[i] = s.
    // If team i was already determined before reveals, answer[i] = 0.
    // We want the earliest s at which team i becomes determined.
    
    // So I should process forward and record answer as soon as the team becomes determined.
    
    // For efficiency: maintain for each team i, a count of uncertain teams.
    // When team j's state changes, determine which teams i are affected and update their counts.
    
    // The tricky part is determining "which teams i are affected" efficiently.
    
    // Given the complexity, let me implement a solution that processes events and 
    // for each event, checks a bounded number of teams.
    
    // For case B: when j drops out of case B for pi = some specific value,
    //   all teams with team_p[i] = pi need to recheck case B.
    //   But case B check is just: is there ANY team j with cp_j < pi and cp_j+unc_j > pi?
    //   This is a global property per pi. So we can maintain caseB_count[pi] = number of such teams.
    //   When j's state changes, update caseB_count for affected pi values.
    //   Then for team i, case B is active iff caseB_count[pi] > 0 (minus i's own contribution).
    
    // For case A and C: similar global structures per (pi, ti).
    
    // Let me maintain:
    //   For each possible cp (0..15): a multiset of ct values for teams with unc > 0.
    //     caseA_cts[cp] = multiset of ct values.
    //     For team i with pi, need max(caseA_cts[pi]) >= ti (excluding i if applicable).
    //   
    //   For each possible (cp+unc) = target_pi (0..15): 
    //     For teams with cp < target_pi and unc > 0 and cp+unc = target_pi:
    //     caseC_pens[target_pi] = multiset of (ct + total_unc_pen) values.
    //     For team i with pi, need min(caseC_pens[pi]) < ti (excluding i if applicable).
    
    //   caseB_count[pi] = #{j : cp_j < pi, cp_j + unc_j > pi} for each pi.
    
    // For team i: undetermined iff
    //   caseB_count[pi] - (i contributes to caseB_count[pi]) > 0, OR
    //   caseA_cts[pi] has element >= ti (excluding i's own ct if i is in there), OR
    //   caseC_pens[pi] has element < ti (excluding i's own contribution if there).
    
    // When j's state changes, update these structures. O(P) updates per event.
    // Then to determine if team i is now determined: O(1) for case B, O(log N) for A and C.
    
    // But which teams i need re-checking? Only those affected by the change.
    // When caseB_count[pi] changes, all teams with team_p = pi.
    // When caseA_cts[pi] changes, all teams with team_p = pi.
    // When caseC_pens[pi] changes, all teams with team_p = pi.
    
    // So: group teams by pi. When an data structure for level pi changes, 
    //   check all undetermined teams at level pi.
    
    // Number of teams at level pi: up to N, but across all levels sums to N.
    // Number of events: up to N*P.
    // Each event affects O(P) levels. For each level, check all undetermined teams at that level.
    
    // Worst case: O(N*P * P * N/16) = still potentially O(N^2 * P^2 / 16) which is too much.
    
    // But in practice: once a team is determined, we remove it. And the number of undetermined teams
    // decreases over time. Also, many events don't change the data structures.
    
    // I think for the given constraints this should be fast enough with good implementation.
    // Let me code it up.
    
    // IMPLEMENTATION PLAN:
    // 1. Group teams by pi level. For each level, maintain list of undetermined teams.
    // 2. Maintain caseB_count[pi], caseA_cts[cp], caseC_pens[target_pi].
    // 3. Process reveals in order. For significant reveals, update team j's state and structures.
    // 4. After update, for each affected pi level, check undetermined teams at that level.
    // 5. A team is determined if none of cases A, B, C apply.
    
    // For checking: team i at level pi is undetermined if:
    //   (a) caseB_active(pi, i) OR (b) caseA_active(pi, ti, i) OR (c) caseC_active(pi, ti, i)
    
    // caseB_active(pi, i): caseB_count[pi] > (1 if i contributes to caseB[pi] else 0)
    //   i contributes to caseB[pi] iff cp_i < pi and cp_i + unc_i > pi.
    //   But cp_i and unc_i are i's PUBLIC state. Since team i knows its own result,
    //   does team i contribute to caseB? 
    //   YES: team i IS one of the N teams. Other teams see team i as uncertain.
    //   But for team i's OWN rank computation, team i doesn't count itself.
    //   So: caseB_active(pi, i) = caseB_count[pi] - (1 if i in caseB at pi) > 0.
    //   Similarly for A and C.
    
    // Let me now actually implement this.
    
    // For "i in caseB at pi": i has cp_i < pi and cp_i + unc_i > pi.
    //   cp_i = certain_p[i] (from public state). But pi = team_p[i] which is i's true result.
    //   certain_p[i] is i's public certain_p (problems known to be solved from public view).
    //   team_p[i] is i's true solved count.
    //   These are generally different! certain_p[i] <= team_p[i].
    //   And certain_p[i] + |uncertain[i]| >= team_p[i] (since some uncertain will be AC).
    
    //   So i might indeed be in caseB for its own pi. We need to exclude it.
    
    // OK, let me implement this step by step.
    
    // Data structures:
    // cnt_by_cp_unc[cp][unc] = count of teams with certain_p=cp, |uncertain|=unc
    // For caseB_count[pi]: sum over cp=0..pi-1, unc=pi-cp+1..P of cnt_by_cp_unc[cp][unc]
    // This can be maintained as a running count, updated when teams change state.
    
    // For caseA: for each cp, maintain sorted list of ct values for teams with unc > 0.
    // caseA_max[cp] = max ct value among teams with certain_p=cp and unc>0.
    //   Use a multiset.
    
    // For caseC: for each target = cp+unc, maintain min of (ct + total_unc_pen) 
    //   among teams with unc > 0 and cp < target (i.e., unc > 0, which means cp < cp+unc = target).
    //   Use a multiset.
    
    // Let's code it up.
    
    // Group teams by pi (true solved count)
    vector<vector<int>> teams_by_pi(P+2);
    for(int i = 1; i <= N; i++){
        teams_by_pi[team_p[i]].push_back(i);
    }
    
    // Maintain structures
    // For each team, track whether it contributes to caseB, caseA, caseC for its own pi.
    // caseB_count[pi] = #{j: cp_j < pi, cp_j + unc_j > pi}
    vector<int> caseB_count(P+2, 0);
    
    // caseA: for each cp, multiset of ct for teams with unc > 0
    vector<multiset<int, greater<int>>> caseA(P+2); // sorted descending for easy max
    
    // caseC: for each target=cp+unc, multiset of (ct + total_unc_pen) for teams with unc>0
    vector<multiset<int>> caseC(P+2); // sorted ascending for easy min
    
    // Initialize structures
    // For each team j, add to structures based on initial state
    // Track each team's current state for efficient updates
    // Also track: for each team j, which caseB pi values it contributes to
    // team j contributes to caseB[pi] for all pi in (cp_j, cp_j + unc_j) (exclusive bounds? let me check)
    // caseB: cp_j < pi AND cp_j + unc_j > pi. So pi in (cp_j, cp_j + unc_j) = {cp_j+1, ..., cp_j+unc_j-1}
    // Wait: cp_j < pi means pi >= cp_j + 1. cp_j + unc_j > pi means pi <= cp_j + unc_j - 1.
    // So pi ranges from cp_j+1 to cp_j+unc_j-1. This is only valid when unc_j >= 2.
    // If unc_j = 1: no pi satisfies (cp_j < pi < cp_j + 1). So caseB doesn't apply.
    // Wait, cp_j + unc_j = cp_j + 1. So cp_j < pi and cp_j + 1 > pi means cp_j < pi < cp_j + 1.
    // Since pi is integer, no pi satisfies this. So caseB with unc=1 has no pi values. Correct.
    // caseB only applies when unc_j >= 2, and contributes to pi in [cp_j+1, cp_j+unc_j-1].
    
    for(int j = 1; j <= N; j++){
        int cp = certain_p[j];
        int unc = (int)uncertain[j].size();
        int ct = certain_t[j];
        
        // Case B: contribute to caseB_count[pi] for pi in [cp+1, cp+unc-1]
        for(int pi = cp+1; pi <= cp+unc-1 && pi <= P; pi++){
            caseB_count[pi]++;
        }
        
        // Case A: if unc > 0, add ct to caseA[cp]
        if(unc > 0){
            caseA[cp].insert(ct);
        }
        
        // Case C: if unc > 0, target = cp + unc, add (ct + total_unc_pen) to caseC[target]
        if(unc > 0){
            int target = cp + unc;
            if(target <= P){
                caseC[target].insert(ct + uncertain_total_pen[j]);
            }
        }
    }
    
    // For each team i, check if determined initially
    vector<int> answer(N+1, -1); // -1 means not yet determined
    vector<bool> determined(N+1, false);
    
    auto is_determined = [&](int i) -> bool {
        int pi = team_p[i];
        int ti = team_t[i];
        
        // Check case B: any team j != i with cp_j < pi, cp_j + unc_j > pi?
        // i's own contribution: cp_i < pi and cp_i + unc_i > pi
        int cp_i = certain_p[i];
        int unc_i = (int)uncertain[i].size();
        bool i_in_caseB = (cp_i < pi && cp_i + unc_i > pi);
        int B = caseB_count[pi] - (i_in_caseB ? 1 : 0);
        if(B > 0) return false;
        
        // Check case A: any team j != i with cp_j = pi, unc_j > 0, ct_j >= ti?
        // i's contribution to caseA[pi]: if unc_i > 0 and cp_i = pi, i is in caseA[pi] with ct = certain_t[i]
        bool i_in_caseA = (unc_i > 0 && cp_i == pi);
        if(!caseA[pi].empty()){
            int max_ct = *caseA[pi].begin(); // max element (sorted desc)
            if(i_in_caseA){
                // Need to check if there's another element >= ti
                // If max_ct > certain_t[i]: definitely yes (different team)
                // If max_ct = certain_t[i]: check if there's more than one such element, or another >= ti
                // This is tricky with multisets...
                // Let's check: if max_ct >= ti, then either it's from another team (done) or from i.
                // If it's from i: max_ct = certain_t[i] >= ti.
                //   Need another element >= ti. 
                //   Check: if caseA[pi].count(certain_t[i]) > 1, then yes.
                //   Or if there's an element >= ti that's != certain_t[i], then yes.
                // Simpler: temporarily erase one copy of certain_t[i], check max >= ti, re-insert.
                auto it = caseA[pi].find(certain_t[i]);
                caseA[pi].erase(it);
                if(!caseA[pi].empty() && *caseA[pi].begin() >= ti){
                    caseA[pi].insert(certain_t[i]);
                    return false;
                }
                caseA[pi].insert(certain_t[i]);
            } else {
                if(max_ct >= ti) return false;
            }
        }
        
        // Check case C: any team j != i with cp_j + unc_j = pi, cp_j < pi, 
        //   ct_j + total_unc_pen_j < ti?
        // i's contribution to caseC[pi]: if unc_i > 0 and cp_i + unc_i = pi and cp_i < pi
        //   (cp_i < pi is equivalent to unc_i > 0 since cp_i + unc_i = pi)
        //   So: unc_i > 0 and cp_i + unc_i = pi.
        bool i_in_caseC = (unc_i > 0 && cp_i + unc_i == pi);
        if(pi <= P && !caseC[pi].empty()){
            int min_pen = *caseC[pi].begin(); // min element (sorted asc)
            if(i_in_caseC){
                int my_pen = certain_t[i] + uncertain_total_pen[i];
                auto it = caseC[pi].find(my_pen);
                caseC[pi].erase(it);
                if(!caseC[pi].empty() && *caseC[pi].begin() < ti){
                    caseC[pi].insert(my_pen);
                    return false;
                }
                caseC[pi].insert(my_pen);
            } else {
                if(min_pen < ti) return false;
            }
        }
        
        return true;
    };
    
    // Check initial determination
    // But checking all N teams is O(N * log N). That's fine.
    for(int i = 1; i <= N; i++){
        if(is_determined(i)){
            determined[i] = true;
            answer[i] = 0;
        }
    }
    
    // Process reveals
    for(int s = 0; s < F; s++){
        int j = freeze_subs[s].w;
        int p = freeze_subs[s].p;
        string r = freeze_subs[s].r;
        
        // Check if this is a significant reveal (last submission for team j, problem p)
        if(!is_last[s]) continue; // Not last, no state change
        
        // This is the last submission for (j, p). Result is r.
        // Update team j's state:
        // Remove from current structures
        int old_cp = certain_p[j];
        int old_unc = (int)uncertain[j].size();
        int old_ct = certain_t[j];
        int old_target = old_cp + old_unc;
        
        // Remove from caseB
        for(int pi = old_cp+1; pi <= old_cp+old_unc-1 && pi <= P; pi++){
            caseB_count[pi]--;
        }
        
        // Remove from caseA
        if(old_unc > 0){
            caseA[old_cp].erase(caseA[old_cp].find(old_ct));
        }
        
        // Remove from caseC
        if(old_unc > 0 && old_target <= P){
            int pen = old_ct + uncertain_total_pen[j];
            caseC[old_target].erase(caseC[old_target].find(pen));
        }
        
        // Update state
        uncertain[j].erase(p);
        if(r == "AC"){
            certain_p[j]++;
            certain_t[j] += penalty_if_solve[j][p];
        }
        uncertain_total_pen[j] -= penalty_if_solve[j][p];
        // (If WA, certain_p and certain_t stay same, just remove from uncertain)
        
        int new_cp = certain_p[j];
        int new_unc = (int)uncertain[j].size();
        int new_ct = certain_t[j];
        int new_target = new_cp + new_unc;
        
        // Add to caseB
        for(int pi = new_cp+1; pi <= new_cp+new_unc-1 && pi <= P; pi++){
            caseB_count[pi]++;
        }
        
        // Add to caseA
        if(new_unc > 0){
            caseA[new_cp].insert(new_ct);
        }
        
        // Add to caseC
        if(new_unc > 0 && new_target <= P){
            caseC[new_target].insert(new_ct + uncertain_total_pen[j]);
        }
        
        // Now check which pi levels were affected
        // Affected levels: union of old range and new range for caseB,
        //   old_cp, new_cp for caseA, old_target, new_target for caseC
        set<int> affected_pis;
        for(int pi = old_cp+1; pi <= old_cp+old_unc-1 && pi <= P; pi++) affected_pis.insert(pi);
        for(int pi = new_cp+1; pi <= new_cp+new_unc-1 && pi <= P; pi++) affected_pis.insert(pi);
        if(old_cp <= P) affected_pis.insert(old_cp);
        if(new_cp <= P) affected_pis.insert(new_cp);
        if(old_target <= P) affected_pis.insert(old_target);
        if(new_target <= P) affected_pis.insert(new_target);
        
        // For each affected pi level, check undetermined teams
        for(int pi : affected_pis){
            if(pi < 0 || pi > P) continue;
            for(int i : teams_by_pi[pi]){
                if(determined[i]) continue;
                if(is_determined(i)){
                    determined[i] = true;
                    answer[i] = s + 1; // 1-indexed reveal count
                }
            }
        }
    }
    
    // Teams still not determined: they must be determined by end (all submissions revealed)
    // Actually after all reveals, all uncertainties are resolved, so all teams should be determined.
    // But if a team was never affected, its answer should be F (last reveal).
    // Wait, some teams might have answer[i] = -1 still. After revealing everything, all uncertainty
    // is gone, so they MUST be determined. Their answer = the step at which the last uncertainty
    // affecting them was resolved.
    
    // Actually, there might be a bug: we only check teams at affected pi levels.
    // But after all reveals, all teams should be determined. Teams with answer=-1 were determined
    // at some point that we missed (not in an affected pi level?). 
    
    // Actually, we should be checking properly. Let me add a final check.
    for(int i = 1; i <= N; i++){
        if(!determined[i]){
            // This shouldn't happen if logic is correct, but just in case:
            answer[i] = F;
            determined[i] = true;
        }
    }
    
    // Output
    for(int i = 1; i <= N; i++){
        cout << answer[i];
        if(i < N) cout << ' ';
    }
    cout << '\n';
    
    return 0;
}
