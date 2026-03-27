#!/usr/bin/python3

import sys
input = sys.stdin.readline

def main():
    N, M = map(int, input().split())

    lost = set()
    weights = [0] * (N + 1)  # 1-indexed

    def query(i, s):
        print(f"? {i} {s}", flush=True)
        resp = input().strip()
        if resp == "borta":
            lost.add(i)
            return True
        return False

    # Upper bound on weight for each index
    hi = [M] * (N + 2)  # hi[i] = upper bound on w_i

    # Process strawberries from right to left.
    # For each alive strawberry i with weight range [lo, hi[i]]:
    #   Query with strength hi[i] - 1.
    #   If kvar: w_i > hi[i]-1, so w_i = hi[i]. Determined.
    #   If borta: w_i <= hi[i]-1. Lost. Tighten hi for all j <= i.
    #
    # Loss bound: each borta reduces the running upper bound by >= 1.
    # Upper bound starts at M and can decrease to 1: at most M-1 losses.
    # Query bound: at most 1 query per strawberry = N <= 300.

    for i in range(N, 0, -1):
        if hi[i] <= 0:
            # Shouldn't happen, but safety
            continue
        if hi[i] == 1:
            # Weight must be 1 (lower bound is 1, upper bound is 1)
            weights[i] = 1
            continue

        s = hi[i] - 1
        if query(i, s):
            # borta: w_i <= s = hi[i] - 1
            # Propagate upper bound leftward
            new_hi = s
            for j in range(i, 0, -1):
                if hi[j] > new_hi:
                    hi[j] = new_hi
                else:
                    break
        else:
            # kvar: w_i = hi[i]
            weights[i] = hi[i]

    # Any remaining alive berry with weight 0 means lo == hi == some value
    # This happens when hi was pushed down to match lo (=1) without querying
    for i in range(1, N + 1):
        if i not in lost and weights[i] == 0:
            weights[i] = hi[i]

    for i in lost:
        weights[i] = -1

    result = " ".join(str(weights[i]) for i in range(1, N + 1))
    print(f"! {result}", flush=True)

main()