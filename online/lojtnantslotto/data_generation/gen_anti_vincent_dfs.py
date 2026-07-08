#!/usr/bin/python3
# Worst case for vincent.rs.
#
# vincent's algorithm: ratio-greedy, then sell back the 7/6/5/4/3 most expensive
# taken items of value 1/2/3/4/5, then DFS-rebuy from the freed budget (dedup on
# (budget, positions)). The DFS cost is the number of reachable states.
#
# The freed budget is at most 65*R (R = frontier ratio): selling 7 items of
# value 1 (weight <= 1*R), 6 of value 2 (<= 2R), 5 of value 3 (<= 3R), 4 of
# value 4 (<= 4R), 3 of value 5 (<= 5R) => 7+12+15+16+15 = 65 R. Every rebuy
# item costs >= R, so the reachable lattice is {a+2b+3c+4d+5e <= ~65}.
#
# To realise that bound we want the greedy frontier to cut every value class at
# the SAME ratio so all 5 sell-back quotas are filled with the most expensive
# possible items, and to have plenty of cheap items just past the frontier in
# every class so the DFS can fan out in all 5 dimensions. We build one item per
# (value v, level r) of weight v*r (ratio exactly r); the greedy then consumes
# whole levels until the budget runs out, hitting the frontier in all 5 classes
# simultaneously. This yields the maximum DFS state count (~1.7*10^5).

import sys, random

def arg(name, default=None):
    for a in sys.argv:
        if a.startswith(name + "="):
            return a.split("=")[1]
    if default is None:
        print("missing", name); sys.exit(1)
    return default

random.seed(int(arg('seed', sys.argv[-1])))
T = int(arg('t', '1000000'))
N = int(arg('n', '200000'))
L = int(arg('L', '400'))          # full levels the greedy consumes (frontier)
beyond = int(arg('beyond', '160'))  # extra levels per class for DFS rebuy room

items = []
for r in range(1, L + beyond + 1):
    for v in range(1, 6):
        items.append((v, v * r))   # ratio r, sorted naturally per class

# Pad up to N with items far beyond the frontier: huge weight so the greedy
# ignores them and the DFS can never afford them (they only cost IO/sort time).
while len(items) < N:
    items.append((5, 1000000))
items = items[:N]

random.shuffle(items)
out = [f"{len(items)} {T}"]
out += [f"{v} {w}" for v, w in items]
sys.stdout.write("\n".join(out) + "\n")
