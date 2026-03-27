#!/usr/bin/env python3
# @EXPECTED_GRADES@ WA WA WA WA WA
#
# Cheese: Computes farthest visible left/right for each position via the
# Cartesian tree, then treats coverage as a contiguous interval and does
# greedy interval covering. Visibility is NOT contiguous in general, so
# this overclaims coverage and underestimates the answer.

import sys


def main():
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))

    if n == 1:
        print(1)
        return

    # Build Cartesian tree (max-heap)
    left = [-1] * n
    right = [-1] * n
    parent = [-1] * n
    stack = []

    for i in range(n):
        last = -1
        while stack and p[stack[-1]] < p[i]:
            last = stack.pop()
        if stack:
            right[stack[-1]] = i
            parent[i] = stack[-1]
        if last != -1:
            left[i] = last
            parent[last] = i
        stack.append(i)

    # For each node, compute farthest visible left and right
    # Visibility set of i: {parent[i], left[i], right[i], i-1, i+1} (where valid)
    # Interval = [min of visible positions, max of visible positions]
    intervals = []
    for i in range(n):
        lo = i
        hi = i
        if parent[i] != -1:
            lo = min(lo, parent[i])
            hi = max(hi, parent[i])
        if left[i] != -1:
            lo = min(lo, left[i])
        if right[i] != -1:
            hi = max(hi, right[i])
        if i > 0:
            lo = min(lo, i - 1)
        if i < n - 1:
            hi = max(hi, i + 1)
        intervals.append((lo, hi))

    # Greedy interval cover of [0, n-1]
    # Sort by left endpoint for the sweeping algorithm
    indexed = sorted(range(n), key=lambda i: intervals[i][0])

    answer = 0
    covered_until = -1
    idx = 0

    while covered_until < n - 1:
        best_right = -1
        while idx < n and intervals[indexed[idx]][0] <= covered_until + 1:
            best_right = max(best_right, intervals[indexed[idx]][1])
            idx += 1
        if best_right == -1:
            break
        covered_until = best_right
        answer += 1

    print(answer)


if __name__ == "__main__":
    main()
