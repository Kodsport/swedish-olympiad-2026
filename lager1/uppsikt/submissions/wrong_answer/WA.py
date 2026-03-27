#!/usr/bin/env python3
# @EXPECTED_GRADES@ WA WA WA WA WA

import sys


def main() -> None:
    input = sys.stdin.readline
    n = int(input())
    p = list(map(int, input().split()))

    left = [-1] * n
    stack = []
    for i, value in enumerate(p):
        while stack and p[stack[-1]] < value:
            stack.pop()
        if stack:
            left[i] = stack[-1]
        stack.append(i)

    right = [n] * n
    stack = []
    for i in range(n - 1, -1, -1):
        value = p[i]
        while stack and p[stack[-1]] < value:
            stack.pop()
        if stack:
            right[i] = stack[-1]
        stack.append(i)

    intervals = [
        (left[i] if left[i] != -1 else 0, right[i] if right[i] != n else n - 1)
        for i in range(n)
    ]
    intervals.sort(key=lambda interval: (interval[1], interval[0]))

    chosen = -1
    answer = 0
    for lo, hi in intervals:
        if chosen < lo:
            chosen = hi
            answer += 1

    print(answer)


if __name__ == "__main__":
    main()
