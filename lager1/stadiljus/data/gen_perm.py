#!/usr/bin/env python3

import math
import random
import sys


def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=", 1)[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default


def alternating_perm(n):
    low = list(range(1, (n + 1) // 2 + 1))
    high = list(range((n + 1) // 2 + 1, n + 1))
    high.reverse()

    perm = []
    for i in range(n):
        if i % 2 == 0:
            perm.append(low[i // 2])
        else:
            perm.append(high[i // 2])
    return perm


def bitonic_perm(n):
    mid = n // 2
    left = list(range(1, mid + 1))
    right = list(range(mid + 1, n + 1))
    right.reverse()
    return left + right


def recursive_perm(n):
    order = []

    def rec(lo, hi):
        if lo > hi:
            return
        mid = (lo + hi) // 2
        order.append(mid)
        rec(lo, mid - 1)
        rec(mid + 1, hi)

    rec(0, n - 1)
    perm = [0] * n
    value = n
    for idx in order:
        perm[idx] = value
        value -= 1
    return perm


def block_perm(n, block_size):
    perm = []
    start = 1
    while start <= n:
        end = min(n, start + block_size - 1)
        perm.extend(range(end, start - 1, -1))
        start = end + 1
    return perm


random.seed(int(cmdlinearg("seed", sys.argv[-1])))
n = int(cmdlinearg("n"))
mode = cmdlinearg("mode")

if mode == "inc":
    perm = list(range(1, n + 1))
elif mode == "dec":
    perm = list(range(n, 0, -1))
elif mode == "alternating":
    perm = alternating_perm(n)
elif mode == "bitonic":
    perm = bitonic_perm(n)
elif mode == "recursive":
    perm = recursive_perm(n)
elif mode == "blocks":
    block_size = int(cmdlinearg("block", max(2, int(math.isqrt(n)))))
    perm = block_perm(n, block_size)
else:
    print("unknown mode", mode)
    sys.exit(1)

assert sorted(perm) == list(range(1, n + 1))
print(n)
print(*perm)
