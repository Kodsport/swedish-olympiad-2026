#!/usr/bin/python3
import sys
input = sys.stdin.readline

def main():
    N, M = map(int, input().split())
    
    lost = set()
    weights = [0] * (N + 1)
    
    def query(i, s):
        print(f"? {i} {s}", flush=True)
        resp = input().strip()
        if resp == "borta":
            lost.add(i)
            return True
        return False
    
    # solve(ilo, ihi, vlo, vhi):
    # All alive berries in [ilo..ihi] have weights in [vlo..vhi].
    # 
    # Key choice: which index to query?
    # If we query the RIGHTMOST alive index with strength vmid:
    #   kvar: w_right > vmid. Since w_right is the max in range,
    #         we can't conclude anything about others being > vmid.
    #         But w_right in [vmid+1, vhi]. Narrow range for right only.
    #   borta: w_right <= vmid. Since sorted, ALL in [ilo..right-1] have w <= vmid.
    #          So entire range [ilo..right-1] has weight in [vlo, vmid]. 
    #          Plus right is lost. No remaining indices with w > vmid in this range!
    #          Wait, but there could be indices after right that we skipped (lost).
    #          No, right is the rightmost alive index. Lost indices after right 
    #          are already gone.
    #          So solve(ilo, right-1, vlo, vmid) and we're done!
    #          But what about weight values in [vmid+1, vhi]? No alive berry has those!
    #          That's fine - they don't exist in this range.
    #   
    # Hmm but this only narrows one berry at a time in the kvar case.
    
    # Better: query index imid (middle) with vmid, but handle it smart.
    # 
    # Actually, let me try querying the LEFTMOST alive index with strength vmid:
    #   kvar: w_left > vmid. Since sorted, ALL in [left+1..ihi] have w > vmid.
    #          So [left+1..ihi] has weight in [vmid+1, vhi].
    #          left itself: w in [vmid+1, vhi].
    #          So [left..ihi] has weight in [vmid+1, vhi].
    #          No berries with weight <= vmid in range!
    #          Solve(left, ihi, vmid+1, vhi). Done! Value range halved, full index range.
    #   borta: w_left <= vmid. left is lost. w_left in [vlo, vmid].
    #          [left+1..ihi]: w >= w_left >= vlo, and w in [vlo, vhi].
    #          Not narrowed for the right side!
    #          Solve(left+1, ihi, vlo, vhi).
    #          Value range NOT narrowed. Index range reduced by 1.
    #
    # This is bad in the borta case: we only reduce index by 1 each time.
    # Worst case: all berries have weight 1. vmid = M/2.
    # Query leftmost: borta (w=1 <= M/2). Lost it. Next: borta again. Etc.
    # Until all with w=1 are exhausted. Total losses could be huge.
    
    # HYBRID: query middle index, but use the SORTED property more aggressively.
    #
    # After borta at imid with strength vmid:
    #   w_imid <= vmid. All j < imid: w_j <= vmid. Range [ilo, imid-1] -> [vlo, vmid].
    #   For j > imid: w_j >= w_imid. w_imid in [vlo, vmid]. So w_j in [vlo, vhi].
    #   NOT narrowed. BUT we know w_j >= w_imid >= vlo. Which we already knew.
    #   
    #   However: we can track that w_{imid+1} >= w_imid. Since imid is lost, 
    #   we don't know w_imid exactly. But w_imid in [vlo, vmid].
    #   So w_{imid+1} in [vlo, vhi]. No help.
    
    # After kvar at imid with strength vmid:
    #   w_imid > vmid, so w_imid in [vmid+1, vhi].
    #   All j > imid: w_j >= w_imid > vmid. Range [imid+1, ihi] -> [vmid+1, vhi].
    #   imid itself: w_imid in [vmid+1, vhi]. Include in right: [imid, ihi] -> [vmid+1, vhi].
    #   For j < imid: w_j <= w_imid <= vhi. w_j in [vlo, vhi]. NOT narrowed.
    
    # So: kvar narrows the RIGHT half (both index and value).
    #     borta narrows the LEFT half (both index and value), but right half stays full.
    
    # For kvar: right gets [vmid+1, vhi] (halved value), left stays [vlo, vhi].
    # For borta: left gets [vlo, vmid] (halved value), right stays [vlo, vhi].
    # One half ALWAYS narrows, the other stays full.
    
    # The "not narrowed" half gets full value range but ~half index range.
    # Eventually it recurse deeper and narrow further.
    
    # T(n, m) = T(n/2, m/2) + T(n/2, m) + 1
    # The heavy path is the chain of T(n/2, m) calls.
    # After k steps on the heavy path: T(n/2^k, m).
    # At each step, a "light" branch T(n/2^k, m/2) splits off.
    # The heavy path terminates at T(1, m) which takes log(m) steps.
    # So total from heavy path: sum_{k=0}^{log n} T(1, m/2^k) + log n + light branches.
    # Hmm, hard to compute. Let me just test empirically with N=M=300.
    
    # For LOSSES: each borta call creates one subproblem with narrowed value range
    # and one with full value range. The narrowed-value subproblem will have fewer
    # losses since its value range is smaller. But the full-value subproblem can 
    # still cause many losses.
    
    # LOSS BOUND: How many losses total?
    # Each loss (borta) at depth d narrows the left half's value range by half.
    # The right half keeps full value range.
    # Total losses across the entire tree... hard to bound tightly.
    
    # OK let me just implement and test various approaches empirically.
    # Let me try the approach of querying the RIGHTMOST alive index with vmid.
    # kvar: we know w_right > vmid, and ALL alive indices in range have w <= w_right.
    #   But this doesn't mean all have w > vmid.
    #   It means the max is > vmid. Some could be <= vmid.
    #   Set solve(ilo, right-1, vlo, vhi) and solve(right, right, vmid+1, vhi).
    #   Wait, right is one berry. 
    #   More useful: after kvar at right, we know w_right in [vmid+1, vhi].
    #   For [ilo, right-1]: weights in [vlo, vhi] (not narrowed).
    #   Hmm, same problem.
    
    # I think the fundamental approach is correct but I need to accept the query count.
    # Let me focus on minimizing LOSSES since that's what fails test 1.
    
    # For losses: choosing which side gets the narrowed value range matters.
    # borta narrows left, kvar narrows right.
    # Losses come from borta events.
    # If we could ensure kvar more often, fewer losses.
    # To encourage kvar: query with a LOW strength (more likely w > s).
    # vmid is the midpoint. If we use vmid = vlo (lowest possible):
    #   Almost always kvar (since w >= vlo, kvar iff w > vlo, borta iff w = vlo).
    #   But value range barely narrows for the "good" side.
    
    # What if we pick vmid based on the index position?
    # If imid is near ilo (left): more likely w is small, so use high vmid to avoid borta.
    # If imid is near ihi (right): more likely w is large, so use low vmid.
    # This is adaptive binary search on values based on index position.
    
    # For uniformly distributed weights in [vlo, vhi] across [ilo, ihi]:
    # Expected weight of index imid: vlo + (vmid - vlo) * (imid - ilo) / (ihi - ilo).
    # We want to set vmid = expected weight of imid.
    # vmid = vlo + (vhi - vlo) * (imid - ilo) / (ihi - ilo).
    # This way, the probability of kvar ≈ borta ≈ 50%, regardless of imid position.
    # This is like interpolation search.
    
    # But we don't control the distribution of weights.
    
    # Let me try the opposite: split the VALUE range proportionally to the INDEX position.
    # imid = (ilo + ihi) // 2. Position fraction: f = (imid - ilo) / (ihi - ilo).
    # vmid = vlo + round(f * (vhi - vlo)).
    # If imid is at middle: vmid is at middle too. Standard.
    # If imid is left-biased (more alive indices on left): vmid is low, more kvar expected.
    
    # Hmm, this is getting complicated. Let me just ensure at most M-1 losses 
    # in a simpler way.
    
    # SIMPLE FIX: track total losses. If we've used M-1 losses already,
    # STOP making queries that could result in borta.
    # How? Only make queries we're sure will return kvar.
    # If we can't determine a weight without risking borta, we have a problem.
    
    # But at that point, all remaining berries should have narrow enough value ranges
    # from the information gathered so far.
    
    # Actually: if we've lost M-1 berries, we can deduce the remaining weights!
    # Because: the sorted array has N berries with weights in [1,M]. 
    # With M-1 losses, we've identified at most M-1 boundary points.
    # Each boundary tells us the weight transition.
    # Actually, each borta event tells us "this berry had w <= s", which combined
    # with sorted order gives group information.
    
    # Hmm, it's not that simple. Let me think about when losses = M-1:
    # We know the weights of M-1 lost berries (each has w <= s_i for their query strength).
    # But we don't know exact weights of lost berries.
    # However, we have narrowed ranges for all alive berries.
    
    # KEY INSIGHT FOR LOSS BOUND:
    # In the D&C tree, borta events happen when we find that w_imid <= vmid.
    # The left subtree then has value range [vlo, vmid] which is half.
    # The right subtree has value range [vlo, vhi] (full).
    # But! The right subtree also has HALF the index range.
    # After log(N) borta events on the "right heavy" path, the index range is 0.
    # And each borta reduces the left side's value range.
    # 
    # Total bortas ≤ N (each borta removes one index). But we need ≤ M-1.
    
    # When M < N, many indices share the same weight. Borta at these just wastes losses.
    # We need to be smarter.
    
    # CORRECT APPROACH: D&C on VALUES, binary search on INDICES.
    # But limit to 1 borta per boundary using the following:
    # After first borta in a search for boundary[v], STOP the binary search.
    # Accept that we don't know boundary[v] exactly - we know boundary[v] >= borta_idx.
    # Then query the NEXT ALIVE index above borta_idx. If kvar: boundary = borta_idx. Done.
    # If that next index is also borta-prone... use it to help.
    
    # Wait, actually: after borta at idx with strength v, boundary[v] >= idx.
    # Now query the next alive index j > idx with strength v:
    #   If kvar: w_j > v, so boundary[v] is between idx and j (exclusive).
    #            boundary[v] = last index in [idx, j-1] with w <= v.
    #            idx was lost. Indices between idx+1 and j-1 are either lost or alive.
    #            If all of [idx+1, j-1] are lost (from previous queries), 
    #            then boundary[v] = idx or one of those lost indices.
    #            We don't need to know exactly since those are all lost (-1).
    #            We just need to know that alive index j has weight > v.
    #            For alive indices < idx: they have w <= vmid (handled by left recursion).
    #            So boundary[v] is somewhere in [idx, j-1], all of which are lost.
    #            We can set boundary[v] = j-1 (or idx, doesn't matter, all are lost between).
    #            
    #   If borta: w_j <= v. boundary[v] >= j. Another loss!
    
    # So we need to keep querying until kvar. Number of consecutive bortas at the 
    # boundary = number of berries at the boundary with weight v.
    # This could be many. E.g., if 100 berries have weight 1 and v=1, 
    # binary search might hit several.
    
    # To minimize: after first borta, jump to the RIGHT end and scan left with kvar.
    # After borta at idx (lo = idx):
    #   Query hi with strength v. 
    #   If kvar: boundary is in [idx, hi-1]. Binary search in [idx, hi-1].
    #            But all queries now should return kvar (everything near hi has w > v).
    #            Binary search: each step returns kvar, halving the range.
    #            Total: O(log(hi-idx)) steps, 0 bortas.
    #   If borta at hi: boundary >= hi. But hi was already the top of range.
    #            boundary[v] = hi. Search done.
    #            2 total bortas for this boundary. Bad if this is the top boundary.
    
    # Hmm, querying hi first INSTEAD of binary search:
    # Query hi with strength v:
    #   kvar: boundary < hi. New hi = hi-1. Need to find exact boundary.
    #         Binary search in [lo, hi-1]. All queries return kvar. O(log N) queries, 0 bortas.
    #   borta: boundary >= hi. Since hi is top: boundary = hi. 1 borta. Done.
    # Then binary search remainder: all kvar.
    # Total per boundary: 1 borta + O(log N) queries.
    # Total bortas across M-1 boundaries: M-1. Total queries: (M-1) * O(log N). Too many!
    
    # With D&C on values: total queries per boundary on average is O(log(N/M)),
    # total = M * log(N/M) which for M=N is 0. Good!
    # For M=N=300: 0 + 300 bortas (one per boundary) = 300 queries.
    # Wait, M-1 = 299 boundaries, each needing >= 1 query (the borta) + binary search.
    # If all ranges are size 1 (M=N, each weight is distinct):
    #   Each boundary search: range = [v-1, v], single query, 1 borta. 
    #   Total: 299 queries, 299 losses. Fits!
    # If M < N, fewer boundaries, larger ranges.
    # For M=2, N=300: 1 boundary. Binary search in [0, 300]. O(log 300) ≈ 9 queries, 1 borta.
    # Great!
    
    # OK so the D&C on values approach with "query hi first, then binary search kvar-only"
    # gives: total queries = sum over boundaries of (1 + log(range)).
    # With D&C sharing ranges: total = O(M + M * log(N/M)).
    # For M=N: 300 + 0 = 300. Barely fits!
    # For M=N/2=150: 150 + 150 * log(2) = 150 + 150 = 300. Fits!
    # For M=1: 0 boundaries. Total = 0. Fits.
    # For M=2, N=300: 1 + log(300) ≈ 10. Fits.
    # Maximum: M + M*log(N/M) is maximized... derivative wrt M: 1 + log(N/M) - 1 = log(N/M).
    # Set = 0: M = N. Value = N. Hmm, actually for M=N: N + N*log(1) = N.
    # For M=N/e: N/e + N/e * 1 = 2N/e ≈ 220. Less than 300.
    # So max is at M=N with value N = 300. Perfect!
    
    # Let me implement this cleanly.
    # D&C on values: solve(vlo, vhi, ilo, ihi).
    # boundary[vlo] = ilo, boundary[vhi] = ihi.
    # vmid = (vlo + vhi) // 2.
    # Binary search for boundary[vmid] in [ilo, ihi].
    # 
    # Step 1: Query the rightmost alive index in [ilo+1, ihi] with strength vmid.
    #   If borta: boundary[vmid] >= that index. If index == ihi: boundary[vmid] = ihi. Done.
    #   If kvar: boundary[vmid] < that index. Set hi = that index - 1.
    #            Now binary search in [ilo, hi] — only kvar responses expected.
    #            Well, not guaranteed... indices in [ilo, hi] could have w <= vmid.
    #            Those would be borta. But we want to find boundary[vmid] = rightmost
    #            with w <= vmid. We've checked everything to the right and it's kvar.
    #            So boundary[vmid] is in [ilo, hi].
    
    # Step 2: Continue binary search in [ilo, hi].
    #   If all remaining indices in range have w > vmid: boundary = ilo (no berries <= vmid).
    #   Otherwise, some have w <= vmid. Binary search finds the boundary.
    #   Each borta finds a berry with w <= vmid (a loss).
    #   After borta at idx: boundary[vmid] >= idx. 
    #   To avoid more bortas: query from the right side.
    
    # Actually let me do it properly:
    # In [lo, hi], binary search for boundary[vmid].
    # Query rightmost alive, call it probe:
    #   kvar: hi = probe - 1 (shrink right, no loss)
    #   borta: lo = probe (confirmed boundary >= probe, 1 loss)
    # After first borta: lo = probe. Now search in [probe, hi].
    # Wait, probe is lost. Search in [probe, hi] for alive indices.
    # But we need to find the rightmost with w <= vmid. Everything right of probe 
    # that's alive: query next rightmost alive. If kvar: boundary is between probe and that.
    # If borta: boundary moves further right.
    
    # The key guarantee: AFTER we find one borta at position `b`, all alive indices
    # ABOVE `b` should return kvar (since we haven't found any above b with w <= vmid yet).
    # Wait, that's not true. There could be alive indices above b with w <= vmid.
    # In a SORTED array: if w_b <= vmid and j > b, then w_j >= w_b >= vlo. But w_j 
    # could still be <= vmid!
    
    # So there's no guarantee that querying above b gives kvar.
    # Multiple bortas can happen.
    
    # To limit to 1 borta per boundary: after borta at b, query from ihi DOWNWARD.
    # We've already confirmed everything above some point is kvar.
    # Hmm, we haven't. We came from above: first queried rightmost, got kvar 
    # (hi narrowed), then queried next rightmost, etc., then got borta at b.
    # So everything above hi+1 (original hi) is kvar. Current search is in [lo, hi].
    # After borta at b: lo = b. Everything in [hi+1, original_hi] is kvar (w > vmid).
    # Everything in [b+1, hi] is unknown!
    
    # OK I think the cleanest solution is:
    # For each boundary search, do normal binary search.
    # Accept multiple bortas per search.
    # But prove total bortas across all searches <= M-1.
    
    # Claim: total bortas across all D&C searches = number of distinct weight groups - 1.
    # Why? Each borta at strength vmid destroys a berry with w <= vmid.
    # The D&C ensures that each "weight group" loses at most one representative.
    # Hmm, not obviously true.
    
    # Let me just try the approach with "query from right" to minimize bortas per search.
    
    # FINAL APPROACH: Binary search from right (query rightmost alive first).
    # After kvar, narrow hi. After borta, narrow lo and switch to querying from
    # the left of the remaining range (since left is more likely kvar now? no...).
    
    # Scrap all that. Let me count losses of my original working approach and 
    # see if I can fix just the loss issue.
    
    # Original test 1 failure: N=5, M=3, w=[1,1,2,3,3]. Lost 3, max 2.
    # The issue: borta at mid of value range AND at left of index range.
    # Two bortas finding the same boundary.
    
    # What if I use vmid = vhi - 1 instead of (vlo+vhi)//2?
    # Then borta means w <= vhi-1. For the RIGHT side: only berries with w = vhi remain.
    # The left side: [vlo, vhi-1]. 
    # This way, each borta peels off exactly the topmost weight group.
    # Total bortas = number of distinct weight values - 1 <= M - 1. 
    
    # Let me check: solve(1, N, 1, M). vmid = M-1.
    # Binary search for the split between w <= M-1 and w = M.
    # Query middle index with strength M-1:
    #   kvar: w > M-1, so w = M. Right half: weight M. Done for right.
    #         Left half: weight in [1, M-1]? No, left half has weight in [1, M].
    #         Hmm, kvar at imid means w_imid = M. All j > imid: w_j = M. Done for right.
    #         Left [ilo, imid-1]: weight in [1, M]. Not narrowed!
    #   borta: w_imid <= M-1. Right half: weight in [1, M]. Left [ilo, imid-1]: weight in [1, M-1].
    
    # Same issue as before. Using vmid = vhi - 1 doesn't help with narrowing.
    
    # THE PROBLEM IS FUNDAMENTAL: one side doesn't narrow in value.
    # Unless we query the SAME index with MULTIPLE strengths.
    # But that wastes queries.
    
    # NEW IDEA: combine the sorted property more aggressively.
    # After kvar at imid with vmid: w_imid > vmid.
    # Also: w_imid <= vhi (from our knowledge).
    # For LEFT side [ilo, imid-1]: w_j <= w_imid <= vhi. And w_j >= vlo.
    # So left side has weight in [vlo, vhi]. Not narrowed.
    # BUT: w_j <= w_imid. And we'll determine w_imid later (recurse on right).
    # Once we know w_imid, we can use it as upper bound for left: [vlo, w_imid].
    # 
    # This is deferred narrowing! But we can't use it in the recursion directly
    # since we determine w_imid by recursing on the right side.
    #
    # APPROACH: Two-phase.
    # Phase 1: Query each alive berry once with strength = mid of its value range.
    #          Propagate bounds using sorted order.
    #          Repeat until all determined.
    # This is iterative refinement.
    
    # Let me implement this iterative approach:
    # Each alive berry i has range [lo_i, hi_i]. Initially [1, M].
    # Propagate sorted: lo_i = max(lo_i, lo_{i-1}), hi_i = min(hi_i, hi_{i+1}).
    # Pick a berry where hi_i - lo_i > 0, query with mid:
    #   kvar: lo_i = mid + 1
    #   borta: (lost)
    # After each query, propagate bounds.
    # Stop when all alive berries have lo_i == hi_i.
    
    # Total queries: each query either kills a berry (at most M-1 times) or 
    # narrows a range by half (at most N * log M total halvings).
    # For N=M=300: 299 + 300 * 9 = 3000. Way too many.
    
    # But with propagation, one kvar can narrow MANY berries.
    # E.g., kvar at i with strength s: lo_i = s+1. Propagate: lo_j = s+1 for all j >= i
    # with current lo_j < s+1. This narrows many at once!
    
    # Similarly, borta at i: hi for all j <= i gets narrowed to s.
    
    # So the total effective "narrowing work" is O(N * log M) but shared across berries.
    # In practice, queries might be much fewer.
    
    # Let me implement this and test.
    
    lo = [1] * (N + 2)  # lo[i] for i in 1..N
    hi = [M] * (N + 2)
    lo[0] = 1
    hi[N + 1] = M
    alive = list(range(1, N + 1))
    
    def propagate():
        # Forward pass: lo[i] >= lo[i-1] (sorted)
        for idx in range(len(alive)):
            i = alive[idx]
            if idx > 0:
                prev = alive[idx - 1]
                lo[i] = max(lo[i], lo[prev])
            if idx < len(alive) - 1:
                nxt = alive[idx + 1]
                hi[i] = min(hi[i], hi[nxt])
        # Backward pass: hi[i] <= hi[i+1]
        for idx in range(len(alive) - 1, -1, -1):
            i = alive[idx]
            if idx < len(alive) - 1:
                nxt = alive[idx + 1]
                hi[i] = min(hi[i], hi[nxt])
            if idx > 0:
                prev = alive[idx - 1]
                lo[i] = max(lo[i], lo[prev])
    
    propagate()
    
    while True:
        # Find an alive berry that's not yet determined
        # Pick one with largest range to maximize info gain
        best = -1
        best_range = 0
        for i in alive:
            r = hi[i] - lo[i]
            if r > best_range:
                best_range = r
                best = i
        
        if best_range == 0:
            break  # all determined
        
        i = best
        s = (lo[i] + hi[i]) // 2
        
        if query(i, s):
            # borta: w_i <= s. 
            # For all j <= i (alive): hi[j] = min(hi[j], s)
            # Actually more precisely: w_i <= s, and since sorted, w_j <= w_i <= s for j <= i.
            # But j could be alive and have hi[j] > s. Narrow it.
            for j in alive:
                if j <= i:
                    hi[j] = min(hi[j], s)
                if j > i:
                    break
            alive.remove(i)
        else:
            # kvar: w_i > s, so w_i >= s+1.
            # For all j >= i: lo[j] = max(lo[j], s+1)
            for j in alive:
                if j >= i:
                    lo[j] = max(lo[j], s + 1)
        
        propagate()
    
    # Assign weights
    for i in alive:
        weights[i] = lo[i]  # lo[i] == hi[i]
    for i in lost:
        weights[i] = -1
    
    result = " ".join(str(weights[i]) for i in range(1, N + 1))
    print(f"! {result}", flush=True)

main()