#!/usr/bin/python3
"""Structured generator for "Långa trappor".

The whole grid is determined by f, since h(x,y) = f[x] - y.  Two facts drive
what makes an input hard:

  * An optimal walk goes up, across, then down, and its cost as a function of
    the peak row is concave -- so the peak is either max(sy,gy) or the top row
    R-1.  Solutions that only ever consider max(sy,gy) are wrong, but only on
    inputs where max(sy,gy) is within ~C rows of the top (climbing k rows costs
    ~k^2 and saves at most (C-2)k).  Hence the `top-K` row specs.
  * f[i] >= R-1, so a large R squeezes every f[i] into a narrow band.  Rich
    column-cost profiles are only possible when R << 10^9, so `profile` and `r`
    have to be chosen together.

Row/column specs accept an integer, or `top`/`top-K`/`mid`/`bot` for rows and
`last`/`mid` for columns.

profile:
  flat      f[i] = R-1 everywhere; the top row is all zeros
  high      f[i] = big everywhere; maximises the answer
  cheap     f[i] = big, except the columns in `cheap=` which are R-1
  nearflat  f[i] = R-1 + small offset, with `bumps` columns pushed up to big
  rand      f[i] uniform in [R-1, big]
"""

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


random.seed(int(cmdlinearg('seed', sys.argv[-1])))

r = int(cmdlinearg('r'))
c = int(cmdlinearg('c'))
profile = cmdlinearg('profile')
big = min(10**9, max(int(cmdlinearg('big', 10**9)), r - 1))
spread = int(cmdlinearg('spread', 20))
bumps = int(cmdlinearg('bumps', 3))


def parse_row(spec):
    if spec == 'top':
        return r - 1
    if spec.startswith('top-'):
        return r - 1 - int(spec[4:])
    if spec == 'mid':
        return r // 2
    if spec == 'bot':
        return 0
    return int(spec)


def parse_col(spec):
    if spec == 'last':
        return c - 1
    if spec == 'mid':
        return c // 2
    return int(spec)


def parse_point(spec):
    col, row = spec.split(',')
    return parse_col(col), parse_row(row)


if profile == 'flat':
    f = [r - 1] * c
elif profile == 'high':
    f = [big] * c
elif profile == 'cheap':
    cheap = [parse_col(x) for x in cmdlinearg('cheap', '0,last').split(',')]
    for x in cheap:
        assert 0 <= x < c, f"cheap column {x} out of range"
    f = [big] * c
    for x in cheap:
        f[x] = r - 1
elif profile == 'nearflat':
    # f_i <= 10^9 caps how far above R-1 we can go; at R = 10^9 that headroom is
    # a single unit, which is all the variation the constraints allow
    room = min(spread, 10**9 - (r - 1))
    f = [r - 1 + random.randint(0, room) for _ in range(c)]
    for _ in range(bumps):
        f[random.randrange(c)] = random.randint(r - 1, big)
elif profile == 'rand':
    f = [random.randint(r - 1, big) for _ in range(c)]
else:
    assert False, f"unknown profile {profile!r}"

assert all(r - 1 <= v <= 10**9 for v in f), f"f out of range: {f}"

sx, sy = parse_point(cmdlinearg('s'))
gx, gy = parse_point(cmdlinearg('g'))

print(r, c)
print(*f)
print(sx, sy)
print(gx, gy)
