#!/usr/bin/python3
#
# Emits one pair of throw lengths.
#
#   mode=fixed  a=A b=B              the exact pair (A, B), used for the grid
#   mode=random group=G idx=I cnt=N  random case number I out of N for group G
#
# The whole input space is only 31*31 = 961 pairs, so the grid is what does the
# actual killing: {0, 1, 10, 20, 21, 22, 23, 30} covers both extremes, a mid
# value and the entire neighbourhood of the 21 metre cutoff, which is every
# value a solution can plausibly branch on.
#
# The random cases exist only so that the data is not exactly the grid.  Each
# group draws from the throw combination it is the first group to allow, with the
# pairs used by fixed test cases removed, and the N cases of a group take one
# pair each from N disjoint slices of that space.  No two test cases in the
# problem can therefore end up with identical input.

import sys
import random


def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default


GRID = [0, 1, 10, 20, 21, 22, 23, 30]
SKOLKVAL = [(21, 3), (10, 10), (30, 0), (22, 30), (29, 29)]
SAMPLES = [(7, 5), (26, 22)]
TAKEN = {(x, y) for x in GRID for y in GRID} | set(SKOLKVAL) | set(SAMPLES)

# What each group is the first to allow.  Group 1 is the base case, group 2 adds
# draws, group 3 adds a single overshoot and group 4 adds two overshoots.
STRATUM = {
    1: lambda a, b: a != b and a <= 21 and b <= 21,
    2: lambda a, b: a == b and a <= 21,
    3: lambda a, b: (a > 21) != (b > 21),
    4: lambda a, b: a > 21 and b > 21,
}

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
mode = cmdlinearg('mode')

if mode == 'fixed':
    a = int(cmdlinearg('a'))
    b = int(cmdlinearg('b'))
elif mode == 'random':
    group = int(cmdlinearg('group'))
    idx = int(cmdlinearg('idx'))
    cnt = int(cmdlinearg('cnt'))
    in_stratum = STRATUM[group]
    space = [(x, y) for x in range(31) for y in range(31)
             if in_stratum(x, y) and (x, y) not in TAKEN]
    if not 0 <= idx < cnt <= len(space):
        print(f"cannot draw case {idx} of {cnt} from {len(space)} pairs")
        sys.exit(1)
    slice_ = space[idx * len(space) // cnt:(idx + 1) * len(space) // cnt]
    a, b = random.choice(slice_)
else:
    print("unknown mode", mode)
    sys.exit(1)

print(a)
print(b)
