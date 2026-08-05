#!/usr/bin/env bash

. ../../../testdata_tools/gen.sh

use_solution joshua.cpp

compile gen_rand.py
compile gen_struct.py

samplegroup
sample 1
sample 2
sample 3

MAXR=1000000000

# The grid is fully determined by f, since the height at (x,y) is f[x] - y.  An
# optimal walk goes up, across, then down, and its cost as a function of the peak
# row is concave -- so the peak is either max(sy,gy) or the top row R-1.  The data
# is built around the ways to get that wrong:
#
#   * always peaking at max(sy,gy)      -> the *-climb* cases.  Only inputs with
#     max(sy,gy) within ~C rows of the top can expose this, since climbing k rows
#     costs ~k^2 and saves at most (C-2)k; the whole gap is at most 20.
#   * always peaking at R-1             -> the *-same* / *-interior cases.
#   * only turning in column sx or gx   -> the *-detour cases.
#   * turning in a single column        -> the *-climb* cases again (crossing the
#     top row needs two distinct turning columns).
#   * anything O(R) in time or memory   -> R = 10^9 in most of groups 1 and 3
#     (the *-exp-* cases are the exception; see the comment there).
#
# f_i >= R-1, so a large R squeezes every f_i into a narrow band (at R = 10^9 only
# two values are left).  Cases that need a rich column-cost profile therefore use
# a smaller R, and are paired with a large-R case covering the same shape.

group group1 20
limits c_is=2
tc_manual ../skolkval_tests/skol_1.in

# C = 2 leaves at most two cells on the crossing row, so climbing above
# max(sy,gy) can never pay off here and the peak is always max(sy,gy).  What is
# left to get wrong is the choice of turning column, the double counting when the
# endpoints share a row or a cell, and any O(R) loop.
# seed 20 keeps the two columns distinct; at R = 10^9 the f_i headroom is a
# single unit, so most seeds collapse to a flat f and duplicate g1-overflow2
tc g1-diag gen_struct r=$MAXR c=2 profile=nearflat s=0,bot g=last,top seed=20
tc g1-antidiag gen_struct r=$MAXR c=2 profile=nearflat s=last,bot g=0,top seed=22
tc g1-samerow gen_struct r=$MAXR c=2 profile=nearflat s=0,mid g=last,mid seed=3
tc g1-samecol gen_struct r=$MAXR c=2 profile=nearflat s=0,bot g=0,top seed=4
tc g1-samecell gen_struct r=$MAXR c=2 profile=nearflat s=1,mid g=1,mid seed=5
# Both endpoints sit in the expensive column, so the entire climb has to happen
# in the other one.
tc g1-detour gen_struct r=100000000 c=2 profile=cheap cheap=last s=0,bot g=0,top seed=6
tc g1-detour2 gen_struct r=100000000 c=2 profile=cheap cheap=0 s=1,bot g=1,top seed=7
# Largest possible answer: a full-height column at f_i = 10^9 sums to ~5*10^17,
# which does not fit in 32 bits.
tc g1-overflow gen_struct r=$MAXR c=2 profile=high s=0,bot g=0,top seed=8
tc g1-overflow2 gen_struct r=$MAXR c=2 profile=high s=0,bot g=last,top seed=9
# f_i = R-1 makes the top row free; endpoints near the top check that this still
# does not make climbing worthwhile when C = 2.
tc g1-flat-top gen_struct r=$MAXR c=2 profile=flat s=0,top-3 g=last,top-3 seed=10
tc g1-flat-attop gen_struct r=$MAXR c=2 profile=flat s=0,top g=last,top seed=11
tc g1-flat-diag gen_struct r=$MAXR c=2 profile=flat s=0,bot g=last,top seed=12
# Degenerate shapes.
tc g1-r1 gen_struct r=1 c=2 profile=high s=0,bot g=last,bot seed=13
tc g1-r2 gen_struct r=2 c=2 profile=flat s=0,bot g=last,top seed=14
tc g1-r1-cell gen_struct r=1 c=2 profile=rand s=1,bot g=1,bot seed=15
for i in {1..8}; do
    tc g1-rand-$i gen_rand r=$MAXR c=2 mode=3x seed=$((100 + i))
done
# mode=exp only bites when R is well below 10^9: f_i >= R-1 pins every f_i to
# R-1 as soon as R-1 exceeds the largest power of `base` that fits in 10^9, which
# is exactly what happens at R = MAXR.  The MAXR version of that shape is already
# covered by g1-flat-* and g1-rand-*, so sweep R alongside the base here instead.
for rb in 1000,1.1 100000,1.5 1000000,2 10000000,3; do
    rr=${rb%,*}; b=${rb#*,}
    tc g1-exp-${b/./_} gen_rand r=$rr c=2 mode=exp base=$b seed=$((200 + ${b/./}))
done
tc g1-small gen_rand r=1000 c=2 mode=3x seed=300

group group2 40
limits maxr=10000
# samples 1 and 2 only; sample 3 has R = 10^5 and belongs to group 3
tc 1
tc 2
tc_manual ../skolkval_tests/skol_2.in
tc_manual ../skolkval_tests/skol_3.in

# R <= 10^4 lets a plain Dijkstra over the whole grid through, so this group is
# about structure rather than size.  Small R also leaves f_i free to range over
# [R-1, 10^9], which is what makes the column-cost profile interesting at all.
#
# Endpoints at top-k with f_i = R-1 in two columns: crossing the free top row
# beats crossing at max(sy,gy), so the peak must be allowed above both endpoints.
tc g2-climb1 gen_struct r=10000 c=10 profile=cheap cheap=0,last s=0,top-1 g=last,top-1 seed=20
tc g2-climb4 gen_struct r=10000 c=10 profile=cheap cheap=0,last s=0,top-4 g=last,top-4 seed=21
tc g2-climb5 gen_struct r=10000 c=10 profile=flat s=0,top-5 g=last,top-5 seed=22
tc g2-climb8 gen_struct r=10000 c=10 profile=flat s=0,top-8 g=last,top-8 seed=23
tc g2-climb-skew gen_struct r=10000 c=10 profile=flat s=0,top-2 g=last,top-7 seed=24
# C = 3 is the smallest width where crossing the top row can save anything.
tc g2-climb-c3 gen_struct r=10000 c=3 profile=flat s=0,top-1 g=last,top-1 seed=25
# Climb still required, but with the columns no longer interchangeable, so the
# pair of turning columns matters as well as the peak row.
tc g2-climb-nf gen_struct r=10000 c=10 profile=nearflat bumps=6 s=0,top-4 g=last,top-4 seed=26
tc g2-climb-nf2 gen_struct r=10000 c=10 profile=nearflat spread=6 s=0,top-3 g=last,top-5 seed=27
# The same climb with the endpoints swapped in row order and in column order, to
# catch a broken start/goal normalisation or column mirror.
tc g2-climb-rev gen_struct r=10000 c=10 profile=cheap cheap=0,last s=last,top-4 g=0,top-4 seed=270
tc g2-climb-skew2 gen_struct r=10000 c=10 profile=flat s=0,top-7 g=last,top-2 seed=271
# Two equally cheap columns with the endpoints on different rows: the cost as a
# function of the turning column has two minima, so it cannot be ternary-searched.
tc g2-twomin gen_struct r=10000 c=10 profile=cheap cheap=3,6 s=0,bot g=last,mid seed=272
tc g2-twomin2 gen_struct r=10000 c=10 profile=cheap cheap=1,8 s=4,bot g=5,top seed=273
# The opposite error: endpoints far from the top, where going up is a disaster.
tc g2-interior gen_struct r=10000 c=10 profile=cheap cheap=3,6 s=0,mid g=last,mid seed=28
tc g2-samerow gen_struct r=10000 c=10 profile=rand s=1,mid g=8,mid seed=29
tc g2-samecol gen_struct r=10000 c=10 profile=rand s=5,bot g=5,top seed=30
tc g2-samecell gen_struct r=10000 c=10 profile=rand s=4,mid g=4,mid seed=31
# Both endpoints in one expensive column, with the only cheap column off to the
# side: the walk has to leave the span between sx and gx entirely.
tc g2-detour gen_struct r=10000 c=10 profile=cheap cheap=0 s=5,bot g=5,top seed=32
tc g2-detour2 gen_struct r=10000 c=10 profile=cheap cheap=last s=2,bot g=3,top seed=33
# Degenerate widths and heights.
tc g2-c1 gen_struct r=10000 c=1 profile=rand s=0,bot g=0,top seed=34
tc g2-c1-cell gen_struct r=10000 c=1 profile=high s=0,mid g=0,mid seed=35
tc g2-r1 gen_struct r=1 c=10 profile=rand s=0,bot g=last,bot seed=36
tc g2-r1-c1 gen_struct r=1 c=1 profile=high s=0,bot g=0,bot seed=37
tc g2-r2 gen_struct r=2 c=10 profile=flat s=0,bot g=last,top seed=38
tc g2-tall-thin gen_struct r=10000 c=2 profile=nearflat s=0,bot g=last,top seed=39
for i in {1..10}; do
    tc g2-rand-$i gen_rand r=10000 c=10 mode=3x seed=$((400 + i))
done
for b in 1.1 1.5 2 3; do
    tc g2-exp-${b/./_} gen_rand r=10000 c=10 mode=exp base=$b seed=$((500 + ${b/./}))
done
for cc in 1 2 3 5 7 9; do
    tc g2-w$cc gen_rand r=10000 c=$cc mode=3x seed=$((600 + cc))
done

group group3 40
include_group sample group1 group2
tc_manual ../skolkval_tests/skol_4.in
tc_manual ../skolkval_tests/skol_5.in

# Everything above at full size.  At R = 10^9 the constraint f_i >= R-1 leaves
# only f_i in {10^9-1, 10^9}, so the climb cases here are decided by differences
# of a few units on top of answers around 10^10 -- a solution that is merely
# close still fails.
tc g3-climb1 gen_struct r=$MAXR c=10 profile=cheap cheap=0,last s=0,top-1 g=last,top-1 seed=40
tc g3-climb2 gen_struct r=$MAXR c=10 profile=cheap cheap=0,last s=0,top-2 g=last,top-2 seed=41
tc g3-climb4 gen_struct r=$MAXR c=10 profile=cheap cheap=0,last s=0,top-4 g=last,top-4 seed=42
tc g3-climb5 gen_struct r=$MAXR c=10 profile=flat s=0,top-5 g=last,top-5 seed=43
tc g3-climb8 gen_struct r=$MAXR c=10 profile=flat s=0,top-8 g=last,top-8 seed=44
tc g3-climb12 gen_struct r=$MAXR c=10 profile=flat s=0,top-12 g=last,top-12 seed=45
tc g3-climb-skew gen_struct r=$MAXR c=10 profile=flat s=0,top-2 g=last,top-9 seed=46
tc g3-climb-c3 gen_struct r=$MAXR c=3 profile=flat s=0,top-1 g=last,top-1 seed=47
# Peak above both endpoints *and* neither turning column at an endpoint.
tc g3-climb-nf gen_struct r=$MAXR c=10 profile=nearflat spread=6 s=0,top-4 g=last,top-4 seed=48
tc g3-climb-nf2 gen_struct r=$MAXR c=10 profile=nearflat bumps=6 s=0,top-3 g=last,top-6 seed=49
# Same climb, endpoints swapped in row order and in column order.
tc g3-climb-rev gen_struct r=$MAXR c=10 profile=cheap cheap=0,last s=last,top-4 g=0,top-4 seed=490
tc g3-climb-skew2 gen_struct r=$MAXR c=10 profile=flat s=0,top-9 g=last,top-2 seed=491
tc g3-climb-rev-nf gen_struct r=$MAXR c=10 profile=nearflat spread=6 s=last,top-5 g=0,top-3 seed=492
# Both endpoints already on the free top row: the answer is 0.
tc g3-attop gen_struct r=$MAXR c=10 profile=flat s=0,top g=last,top seed=50
tc g3-attop-hi gen_struct r=$MAXR c=10 profile=high s=0,top g=last,top seed=51
# Endpoints far from the top, where the peak must stay at max(sy,gy).
tc g3-diag gen_struct r=$MAXR c=10 profile=nearflat s=0,bot g=last,top seed=52
tc g3-antidiag gen_struct r=$MAXR c=10 profile=nearflat s=last,bot g=0,top seed=53
tc g3-samerow gen_struct r=$MAXR c=10 profile=nearflat s=1,mid g=8,mid seed=54
tc g3-samecol gen_struct r=$MAXR c=10 profile=nearflat s=5,bot g=5,top seed=55
tc g3-samecell gen_struct r=$MAXR c=10 profile=nearflat s=3,mid g=3,mid seed=56
tc g3-overflow gen_struct r=$MAXR c=10 profile=high s=0,bot g=0,top seed=57
tc g3-overflow2 gen_struct r=$MAXR c=10 profile=high s=0,bot g=last,top seed=58
# R around 10^8 is the sweet spot: still far too large for anything O(R), but
# f_i may now span a factor of ten, so the column-cost profile has real shape.
tc g3-interior gen_struct r=100000000 c=10 profile=cheap cheap=3,6 s=0,mid g=last,mid seed=59
# Two equally cheap columns with the endpoints on different rows: two minima in
# the turning column, so no ternary search over it.
tc g3-twomin gen_struct r=100000000 c=10 profile=cheap cheap=3,6 s=0,bot g=last,mid seed=590
tc g3-twomin2 gen_struct r=100000000 c=10 profile=cheap cheap=1,8 s=4,bot g=5,top seed=591
tc g3-detour gen_struct r=100000000 c=10 profile=cheap cheap=0 s=5,bot g=5,top seed=60
tc g3-detour2 gen_struct r=100000000 c=10 profile=cheap cheap=last s=2,bot g=3,top seed=61
tc g3-rand-hi gen_struct r=100000000 c=10 profile=rand s=0,bot g=last,top seed=62
tc g3-c1 gen_struct r=$MAXR c=1 profile=high s=0,bot g=0,top seed=63
for i in {1..10}; do
    tc g3-rand-$i gen_rand r=$MAXR c=10 mode=3x seed=$((700 + i))
done
# As in group 1: an exponential f profile is impossible at R = MAXR, since
# f_i >= R-1 leaves no room for it.  Sweep R together with the base, largest R
# with the largest base so that every case keeps a real spread in f.
for rb in 1000,1.1 100000,1.5 1000000,1.7 10000000,2 100000000,3; do
    rr=${rb%,*}; b=${rb#*,}
    tc g3-exp-${b/./_} gen_rand r=$rr c=10 mode=exp base=$b seed=$((800 + ${b/./}))
done
for rr in 100000000 500000000 999999999; do
    tc g3-r$rr gen_rand r=$rr c=10 mode=3x seed=$((900 + rr % 97))
done
for cc in 1 2 3 5 7 9; do
    tc g3-w$cc gen_rand r=$MAXR c=$cc mode=3x seed=$((1000 + cc))
done
