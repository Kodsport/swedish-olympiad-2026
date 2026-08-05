#!/usr/bin/python3
"""Adversarial structures for arborist: coordinate ties with the heavy
saplings listed first, 40 kg saplings interleaved between 20 kg ones, and
saturated/degenerate shapes.  See mode comments below."""
import sys
import random

MAXX = 30


def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default


random.seed(int(cmdlinearg('seed', sys.argv[-1])))
mode = cmdlinearg('mode')
n = int(cmdlinearg('n'))
k = int(cmdlinearg('k'))
c40 = int(cmdlinearg('c40', 0))

assert 0 <= c40 <= n
# A 40 kg sapling needs K >= 40, since the statement guarantees k_i <= K.
assert c40 == 0 or k >= 40, "cannot place 40 kg saplings when k < 40"

if mode == 'ties':
    # Many saplings share a coordinate.  The farthest sapling is alone at its
    # coordinate and weighs 20 kg, every 40 kg sapling shares its coordinate
    # with a 20 kg one, and at each coordinate the 40s are listed first.  So a
    # greedy that picks the farthest coordinate that still fits, but then takes
    # the first sapling standing at that coordinate, loads 20+40 kg and reports
    # a too-small answer.
    distinct = int(cmdlinearg('distinct'))
    lo = int(cmdlinearg('lo', 1))
    hi = int(cmdlinearg('hi', MAXX))
    assert 2 <= distinct <= min(n, hi - lo + 1)
    assert c40 <= n - distinct, "every 40 must share its coordinate with a 20"
    pool = sorted(random.sample(range(lo, hi + 1), distinct), reverse=True)
    mult = [1] * distinct
    for _ in range(n - distinct):
        # The farthest coordinate deliberately keeps a single sapling.
        mult[random.randrange(1, distinct)] += 1
    w_at = [[20] * m for m in mult]
    slack = [(j, s) for j in range(1, distinct) for s in range(mult[j] - 1)]
    random.shuffle(slack)
    assert c40 <= len(slack), "shape has no room for that many 40s"
    for (j, s) in slack[:c40]:
        w_at[j][s] = 40
    slots = list(range(n))
    random.shuffle(slots)
    trees = [None] * n
    at = 0
    for j in range(distinct):
        grp = sorted(slots[at:at + mult[j]])
        at += mult[j]
        for i, w in zip(grp, sorted(w_at[j], reverse=True)):
            trees[i] = (pool[j], w)
    assert None not in trees
elif mode == 'alt':
    # Distinct coordinates, and in descending order the weights alternate
    # 20, 40, 20, 40, ...  Every 40 kg sapling sits between two 20 kg ones, so
    # a greedy that abandons a trip as soon as the farthest remaining sapling
    # does not fit -- or that only pairs saplings adjacent in sorted order --
    # never pairs anything.
    hi = int(cmdlinearg('hi', MAXX))
    assert n <= hi
    assert 2 * c40 <= n, "not enough 20s to interleave the 40s with"
    coords = sorted(random.sample(range(1, hi + 1), n), reverse=True)
    weights = [20] * n
    for i in range(c40):
        weights[2 * i + 1] = 40
    trees = list(zip(coords, weights))
    random.shuffle(trees)
elif mode == 'spread':
    # Distinct coordinates and an exact number of 40 kg saplings, placed at
    # random.  With c40 = n nothing can ever share a trip even though K >= 40.
    lo = int(cmdlinearg('lo', 1))
    hi = int(cmdlinearg('hi', MAXX))
    assert n <= hi - lo + 1
    coords = random.sample(range(lo, hi + 1), n)
    weights = [40] * c40 + [20] * (n - c40)
    random.shuffle(weights)
    trees = list(zip(coords, weights))
elif mode == 'uniform':
    # Every sapling at the same coordinate: only the number of trips matters.
    x = int(cmdlinearg('x'))
    weights = [40] * c40 + [20] * (n - c40)
    random.shuffle(weights)
    trees = [(x, w) for w in weights]
else:
    assert 0, "unknown mode " + mode

assert len(trees) == n
print(n, k)
print(*[t[0] for t in trees])
print(*[t[1] for t in trees])
