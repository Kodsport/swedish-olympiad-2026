#!/usr/bin/python3
import sys

def main():
    input = sys.stdin.readline
    N = int(input().strip())
    a = list(map(int, input().split()))

    # tops: last element of each pile (nonincreasing)
    tops = []
    piles = []  # piles[i] will be the i-th kept-list (one per round)

    for x in a:
        # find smallest index j such that tops[j] < x
        lo, hi = 0, len(tops)
        while lo < hi:
            mid = (lo + hi) // 2
            if tops[mid] < x:
                hi = mid
            else:
                lo = mid + 1
        j = lo
        if j == len(tops):
            # create new pile
            tops.append(x)
            piles.append([x])
        else:
            # append to existing pile j and update its top
            piles[j].append(x)
            tops[j] = x

    out = []
    for p in piles:
        out.append(" ".join(map(str, p)))
    sys.stdout.write("\n".join(out))

if __name__ == "__main__":
    main()
