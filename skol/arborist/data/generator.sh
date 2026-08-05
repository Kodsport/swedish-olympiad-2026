#!/usr/bin/env bash

. ../../../testdata_tools/gen.sh

use_solution joshua_greedy.py

compile gen_rand.py
compile gen_adv.py

samplegroup
sample 1
sample 2

group group1 20
limits k_25=1
tc_manual ../skolkval_tests/skol_1.in
for i in {1..15}; do
    tc g1-$i gen_rand k=25 n=8
done
# K = 25 forces every sapling to weigh 20 kg, so only the shape can vary.
tc g1-ties gen_adv mode=ties n=8 distinct=2 k=25
tc g1-alt gen_adv mode=alt n=8 k=25
tc g1-max gen_adv mode=uniform n=8 x=30 k=25
tc g1-one gen_adv mode=uniform n=1 x=1 k=25
tc g1-small gen_rand k=25 n=3

group group2 20
limits weight_20=1
include_group group1
tc_manual ../skolkval_tests/skol_2.in
for i in {1..10}; do
    tc g2-$i gen_rand k=20 n=8 weight_20=1
done

for i in {11..20}; do
    tc g2-$i gen_rand k=40 n=8 weight_20=1
done

# Every sapling weighs 20 kg here, so K is the interesting axis: two saplings
# fit exactly when K >= 40.  K = 39 kills every "pair when K >= T" for T <= 39
# and K = 40/41 kills every T >= 41.
tc g2-k39 gen_adv mode=ties n=7 distinct=3 k=39
tc g2-k39b gen_adv mode=uniform n=8 x=30 k=39
tc g2-k30 gen_adv mode=alt n=5 k=30
tc g2-k21 gen_adv mode=ties n=8 distinct=4 k=21
tc g2-k41 gen_adv mode=alt n=8 k=41
tc g2-k50 gen_adv mode=uniform n=7 x=30 k=50
tc g2-k50b gen_adv mode=ties n=8 distinct=2 k=50
tc g2-one gen_adv mode=uniform n=1 x=30 k=50
tc g2-two gen_adv mode=spread n=2 k=40 lo=29 hi=30
for kk in 21 30 39 41 45 50; do
    tc g2-r$kk gen_rand k=$kk n=8 weight_20=1
done
tc g2-r3 gen_rand k=39 n=3 weight_20=1
tc g2-r5 gen_rand k=50 n=5 weight_20=1

group group3 20
limits same_coord=1
tc_manual ../skolkval_tests/skol_3.in
# With K = 20 and one shared coordinate the answer only depends on n and x, so
# vary n instead of generating ten copies of the same shape.
for i in {1..10}; do
    tc g3-$i gen_rand k=20 n=$(( (i - 1) % 8 + 1 )) same_coord=1
done

for i in {11..20}; do
    tc g3-$i gen_rand k=40 n=8 same_coord=1
done

# All saplings at one coordinate: the answer is 2n + 2x * (#40 + ceil(#20 / 2)),
# so cover both parities of #20, the all-40 case, and K on either side of 40.
tc g3-k39 gen_adv mode=uniform n=7 x=30 k=39
tc g3-k41 gen_adv mode=uniform n=8 x=17 c40=2 k=41
tc g3-k50 gen_adv mode=uniform n=8 x=30 c40=3 k=50
tc g3-all40 gen_adv mode=uniform n=8 x=30 c40=8 k=41
tc g3-x1 gen_adv mode=uniform n=8 x=1 c40=1 k=45
tc g3-one gen_adv mode=uniform n=1 x=1 c40=1 k=50
for kk in 30 45 50; do
    tc g3-r$kk gen_rand k=$kk n=8 same_coord=1
done
tc g3-r3 gen_rand k=50 n=3 same_coord=1

group group4 40
include_group sample group1 group2 group3
tc_manual ../skolkval_tests/skol_4.in
tc_manual ../skolkval_tests/skol_5.in

for i in {1..10}; do
    tc g4-$i gen_rand k=20 n=8
done

for i in {11..20}; do
    tc g4-$i gen_rand k=40 n=8
done

# Coordinate collisions with the 40 kg saplings listed ahead of the 20 kg ones
# at the same coordinate, and an odd number of 20s so the pairing parity matters.
tc g4-ties gen_adv mode=ties n=8 distinct=3 c40=5 k=40
tc g4-ties2 gen_adv mode=ties n=8 distinct=2 c40=3 k=50
tc g4-ties3 gen_adv mode=ties n=8 distinct=5 c40=3 k=42
tc g4-ties4 gen_adv mode=ties n=7 distinct=3 c40=4 k=41 lo=24 hi=30
# 40 kg saplings wedged between 20 kg ones in coordinate order.
tc g4-alt gen_adv mode=alt n=8 c40=4 k=41
tc g4-alt2 gen_adv mode=alt n=7 c40=3 k=50
# Nothing may share a trip, and a lone 20 kg sapling that must travel alone.
tc g4-all40 gen_adv mode=spread n=8 c40=8 k=40
tc g4-one20 gen_adv mode=spread n=8 c40=7 k=50
tc g4-one gen_adv mode=spread n=1 c40=1 k=40
for kk in 41 45 50; do
    tc g4-r$kk gen_rand k=$kk n=8
done
tc g4-r2 gen_rand k=45 n=2
tc g4-r3 gen_rand k=41 n=3
tc g4-r5 gen_rand k=50 n=5
tc g4-r7 gen_rand k=42 n=7
