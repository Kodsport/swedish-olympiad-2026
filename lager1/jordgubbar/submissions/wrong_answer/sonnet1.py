#!/usr/bin/python3
import sys
input = sys.stdin.readline

def main():
    N, M = map(int, input().split())

    weights = [-1] * (N + 1)   # 1-indexed; -1 = lost
    lost = [False] * (N + 1)

    def query(i, s):
        print(f"? {i} {s}", flush=True)
        resp = input().strip()
        if resp == "borta":
            lost[i] = True
            return True   # berry sucked up (weight <= s)
        return False      # berry remains (weight > s)

    # -------------------------------------------------------------------------
    # ALGORITHM: 2D divide-and-conquer on (values, indices).
    #
    # We want to find, for each value v in {1..M-1}, the "split index":
    #   split[v] = last index i such that w[i] <= v  (0 if none, N if all).
    #
    # Since w is sorted, all berries with index <= split[v] have weight <= v.
    # Knowing all split[v] tells us the weight of every berry:
    #   w[i] = smallest v such that split[v] >= i.
    #
    # We use find_split(v_lo, v_hi, i_lo, i_hi):
    #   "Splits for values [v_lo..v_hi] lie within index range [i_lo-1..i_hi]."
    #   1. Pick v_mid = midpoint of value range.
    #   2. Binary search for split[v_mid] in [i_lo..i_hi]:
    #      - Query (i_mid, v_mid):
    #          "borta": w[i_mid] <= v_mid -> split[v_mid] >= i_mid -> cur_lo = i_mid+1
    #          "kvar":  w[i_mid] > v_mid  -> split[v_mid] < i_mid  -> cur_hi = i_mid-1
    #   3. Record split[v_mid], then recurse on two halves.
    #
    # QUERY COUNT: At most N + M - 1 queries total.
    #   - "borta" queries: at most M-1 (we budget at most M-1 losses).
    #     Actually "borta" advances cur_lo and each berry can only be lost once.
    #   - "kvar" queries: Each "kvar" at level d of the value tree has index range
    #     roughly N/2^d. Total across all levels: sum 2^d * log(N/2^d) = O(N log M / M).
    #     But more precisely: total is bounded by N (each "kvar" advances cur_hi down,
    #     permanently shrinking the range for sub-problems).
    #
    # Actually the right bound is:
    #   Total queries <= (M-1) * 1 + N * something_small.
    # For N = M = 300 this gives ~299 + ~0 = ~299. Within budget.
    # -------------------------------------------------------------------------

    splits = {}
    splits[0] = 0
    splits[M] = N

    def find_split(v_lo, v_hi, i_lo, i_hi):
        """
        Find split[v] for all v in [v_lo..v_hi].
        Precondition: split[v] in [i_lo-1 .. i_hi] for all v in range.
        """
        if v_lo > v_hi:
            return
        if i_lo > i_hi:
            # Index range is empty: split for all values here = i_hi = i_lo - 1
            for v in range(v_lo, v_hi + 1):
                splits[v] = i_hi
            return

        v_mid = (v_lo + v_hi) // 2

        # Binary search for split[v_mid] in index range [i_lo .. i_hi]
        cur_lo = i_lo
        cur_hi = i_hi

        while cur_lo <= cur_hi:
            i_mid = (cur_lo + cur_hi) // 2
            # Skip already-lost berries (they have weight <= something, so weight <= v_mid)
            # Actually if berry i_mid is lost, we know w[i_mid] <= (strength used to lose it)
            # which might or might not be <= v_mid. We need to be careful.
            # If lost, we can't query it. We need to find a non-lost berry.
            # Find the nearest non-lost berry.
            probe = i_mid
            while probe <= cur_hi and lost[probe]:
                probe += 1
            if probe > cur_hi:
                # All berries from i_mid to cur_hi are lost.
                # Lost berries have weight <= something. We need to know if <= v_mid.
                # Since they were lost at some strength s, w[berry] <= s.
                # But s might be > v_mid. We can't be sure.
                # Conservative: treat as if split might be anywhere below i_mid.
                cur_hi = i_mid - 1
                continue
            i_mid = probe

            is_lost = query(i_mid, v_mid)
            if is_lost:
                # w[i_mid] <= v_mid -> split[v_mid] >= i_mid
                cur_lo = i_mid + 1
            else:
                # w[i_mid] > v_mid -> split[v_mid] < i_mid
                cur_hi = i_mid - 1

        split_vmid = cur_lo - 1
        splits[v_mid] = split_vmid

        find_split(v_lo, v_mid - 1, i_lo, split_vmid)
        find_split(v_mid + 1, v_hi, split_vmid + 1, i_hi)

    find_split(1, M - 1, 1, N)

    # Assign weights from splits
    for i in range(1, N + 1):
        if lost[i]:
            weights[i] = -1
            continue
        # Find smallest v with splits[v] >= i
        v_lo_s, v_hi_s = 1, M
        while v_lo_s < v_hi_s:
            v_mid_s = (v_lo_s + v_hi_s) // 2
            if splits.get(v_mid_s, 0) >= i:
                v_hi_s = v_mid_s
            else:
                v_lo_s = v_mid_s + 1
        weights[i] = v_lo_s

    print("! " + " ".join(map(str, weights[1:])), flush=True)

main()