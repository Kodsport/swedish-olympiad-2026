#!/usr/bin/python3
# Worst case (slowest run) for submissions/accepted/vlad2.
#
# vlad2's running time is almost entirely DATA-INDEPENDENT:
#   * it always zero-initialises mx[5*A+1] with A=1e6  -> a 20 MB array, and
#   * it always runs the prefix-max loop `for i in 1..5*A` -> a fixed 5*10^6
#     iterations,
#   * the 6^5 = 7776 combination build and the final 7776 loop are constant.
# These constants cost ~40 ms regardless of the input. The ONLY data-dependent
# work is: reading the N castles, the std::sort of the N indices by ratio
# l_i/g_i, and the O(N) greedy. So to make vlad2 as slow as possible we push
# every one of those to its worst case:
#
#   * N = 2*10^5 (maximum) -> most parsing / sorting / greedy work.
#   * g_i drawn from [10^5, 10^6] -> every g is 6-7 digits, maximising the
#     cin>> parse cost (small g would parse faster).
#   * l_i uniform in {1..5} together with distinct random g -> the ratios
#     l_i/g_i are essentially all DISTINCT. This is what makes the sort slowest:
#     measured min times were ~56 ms for random distinct ratios versus ~40 ms
#     when the ratios are equal or the data is (anti-)sorted. Equal ratios let
#     introsort collapse equal runs and predict branches; a pre-sorted ratio
#     order lets it finish in ~n comparisons. Distinct, shuffled ratios force
#     ~1.4 n log n comparisons AND scatter the l[]/g[] accesses (the sort
#     permutes the index array, so the comparator chases two ~0.8 MB arrays at
#     random -> L2 cache misses on every comparison).
#
# There is nothing special to "special-case" away here: it is just maximal-size,
# maximal-entropy data, so a contestant cannot dodge the case without making
# their solution genuinely fast on arbitrary input.

import sys, random

def arg(name, default=None):
    for a in sys.argv:
        if a.startswith(name + "="):
            return a.split("=")[1]
    if default is None:
        print("missing", name); sys.exit(1)
    return default

random.seed(int(arg('seed', sys.argv[-1])))
n = int(arg('n', 200000))
t = int(arg('t', 1000000))

out = [f"{n} {t}"]
for _ in range(n):
    l = random.randint(1, 5)
    g = random.randint(100000, 1000000)
    out.append(f"{l} {g}")
sys.stdout.write("\n".join(out) + "\n")
