#!/usr/bin/python3
# Tightest-possible tests for vincent.rs's per-value sell-back quotas.
#
# vincent.rs: ratio-greedy, then per value class v=1..5 it sells back the
# quota_v = [7,6,5,4,3] most expensive taken items, then DFS-rebuys (cheapest
# first, unbounded). Its reachable solution space is exactly the box
# { n_v >= greedy_v - quota_v for all v }, so it is wrong iff EVERY optimal
# solution drops more than quota_v items of some class v.
#
# The maximum number of class-v drops that any instance can force is
# [4,4,4,4,3] for v=[1..5] (a swap argument shows more is impossible: replacing
# a non-v sub-multiset of value m*v by m dropped class-v items is always
# weight-feasible, and for k=quota forced drops the "swap-free" non-v value
# caps at 0/5/10 for v=1/2/3, below greedy's k*v; exhaustive 2-type search
# concurs). mode=l<v> generates an instance forcing exactly that many drops,
# so the test suite punishes ANY quota vector not >= [4,4,4,4,3].
#
# Template (v in 1..4): 4 class-v items (best ratio, greedy takes all) +
# k fives (worse ratio). Optimum is the pure-five solution 5k: keeping j>=1
# class-v items needs value 5k - v*j from fives, but v*j is not divisible by 5
# for j=1..4, so EVERY value-5k solution has n_v = 0 => all 4 must be sold
# back. The weight a of class-v items sits in a window making each keep-j
# alternative strictly worse: keeping j items must displace
# d_j = ceil((v*j+1)/5) fives, i.e. j*a > r0 + (d_j - 1)*c where c ~ five
# weight and r0 = T - sum(fives) is the leftover:
#   v=1: a in (r0, c/5)         -> a ~ 150 (c=1000)
#   v=2: a in ((r0+c)/3, 2c/5)  -> a ~ 380
#   v=3: a in ((r0+c)/2, 3c/5)  -> a ~ 560
#   v=4: a in ((r0+3c)/4, 4c/5) -> a ~ 775
# mode=l5 is the mirror: 3 fives (best ratio) + k fours; 5j is not divisible
# by 4 for j=1..3 (4 drops would need supply 4, but then j=4 swaps back),
# five-weight e in (r0 + f, 5f/4) -> e ~ 1230 (f=1000).
#
# The forced drop is number-theoretic (mod-5 / mod-4 representability), not an
# artifact of exact weights, so jitter/scale/T keep it; a contestant cannot
# dodge these tests without genuinely enlarging the sell-back box to
# [4,4,4,4,3] or more.
#
# Inert padding: noise items with weight > T can never be bought by anything,
# so the answer is provably unchanged while n becomes non-trivial.

import sys, random

def arg(name, default=None):
    for a in sys.argv:
        if a.startswith(name + "="):
            return a.split("=")[1]
    if default is None:
        print("missing", name); sys.exit(1)
    return default

random.seed(int(arg('seed', sys.argv[-1])))
mode = arg('mode')          # l1..l5
n = int(arg('n', 100000))   # total items incl. noise
k = int(arg('k', 900))      # size of the big (optimal) class

items = []
if mode in ('l1', 'l2', 'l3', 'l4'):
    v = int(mode[1])
    base = {1: 150, 2: 380, 3: 560, 4: 775}[v]
    fives = [1000 + random.randint(-5, 5) for _ in range(k)]
    gadget = [base + random.randint(0, 6) for _ in range(4)]
    T = sum(fives) + random.randint(40, 60)
    items += [(5, c) for c in fives]
    items += [(v, a) for a in gadget]
elif mode == 'l5':
    fours = [1000 + random.randint(-5, 5) for _ in range(k)]
    fives = [1230 + random.randint(0, 6) for _ in range(3)]
    T = sum(fours) + random.randint(40, 60)
    items += [(4, f) for f in fours]
    items += [(5, e) for e in fives]
else:
    assert 0

# inert noise: weight > T, can never be afforded by any solution
while len(items) < n:
    items.append((random.randint(1, 5), random.randint(T + 1, 1000000)))

random.shuffle(items)
print(len(items), T)
for v_, g_ in items:
    print(v_, g_)
