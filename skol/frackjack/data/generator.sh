#!/usr/bin/env bash
. ../../../testdata_tools/gen.sh

use_solution joshua.py

compile gen_pair.py

# Every throw length a solution can plausibly branch on: both extremes, a mid
# value, and the whole neighbourhood of the 21 metre cutoff.  Each group gets
# the full cross product of the grid values it is allowed to use, which kills
# every wrong solution that any input in the group could kill (see
# data_generation/coverage.py).
GRID="0 1 10 20 21 22 23 30"
GRID_UNDER="0 1 10 20 21"   # the grid values that are legal throws
GRID_OVER="22 23 30"        # the grid values that overshoot the post

# Grid pairs that a skolkval test case already covers in the same group, so that
# no two test cases have identical input.
SKIP="10-10 30-0 22-30"

skip () {
    [[ " $SKIP " == *" $1-$2 "* ]]
}

samplegroup
sample 1
sample 2

# A != B and both throws land at or before the post, so the longer throw always
# wins: no overshoot rule and no draw to handle.
group group1 20
limits a_neq_b=1 maxval=21
tc 1
tc_manual ../skolkval_tests/skol_1.in
for a in $GRID_UNDER; do
    for b in $GRID_UNDER; do
        if [[ $a != $b ]]; then
            tc g1-$a-$b gen_pair mode=fixed a=$a b=$b
        fi
    done
done
for i in {0..5}; do
    tc g1-rand$i gen_pair mode=random group=1 idx=$i cnt=6
done

# Still no overshoots, but A == B is now allowed, so Jack wins the draws.
group group2 20
limits maxval=21
include_group group1
tc_manual ../skolkval_tests/skol_2.in
for a in $GRID_UNDER; do
    if ! skip $a $a; then
        tc g2-$a-$a gen_pair mode=fixed a=$a b=$a
    fi
done
for i in {0..5}; do
    tc g2-rand$i gen_pair mode=random group=2 idx=$i cnt=6
done

# Jack never wins here, so exactly one throw may overshoot.  These are the cases
# that punish comparing raw throw lengths, or measuring the distance to the post
# as |21 - x| instead of discarding overshoots: e.g. (22, 20) is won by Bertil,
# but those two mistakes answer Alice and Jack respectively.
group group3 20
limits jack_not_win=1
include_group group1
tc_manual ../skolkval_tests/skol_3.in
for a in $GRID; do
    for b in $GRID; do
        # Exactly one throw overshoots; pairs where both are legal are already
        # covered by group1, and pairs where both overshoot would let Jack win.
        if [[ $a != $b ]] && [[ $a -gt 21 || $b -gt 21 ]] && [[ $a -le 21 || $b -le 21 ]] \
                && ! skip $a $b; then
            tc g3-$a-$b gen_pair mode=fixed a=$a b=$b
        fi
    done
done
for i in {0..5}; do
    tc g3-rand$i gen_pair mode=random group=3 idx=$i cnt=6
done

# No restrictions: adds the pairs where both throws overshoot, so Jack wins even
# when the two throws differ.
group group4 40
include_group group2 group3
tc 2
tc_manual ../skolkval_tests/skol_4.in
tc_manual ../skolkval_tests/skol_5.in
for a in $GRID_OVER; do
    for b in $GRID_OVER; do
        if ! skip $a $b; then
            tc g4-$a-$b gen_pair mode=fixed a=$a b=$b
        fi
    done
done
for i in {0..7}; do
    tc g4-rand$i gen_pair mode=random group=4 idx=$i cnt=8
done
