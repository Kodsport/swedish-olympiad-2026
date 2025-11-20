#!/bin/bash
. ../../../testdata_tools/gen.sh

use_solution ../z3_chatgpt.py cpython3

compile gen_rand.py

#samplegroup

group group1 20
for i in {1..10}; do
    tc g1-$i gen_rand r=5 c=5 p_blocked=0.3 max_coord=500
done

for i in {11..20}; do
    tc g1-$i gen_rand r=5 c=5 p_blocked=0.4 max_coord=500
done

for i in {21..30}; do
    tc g1-$i gen_rand r=5 c=5 p_blocked=0.5 max_coord=500
done


group group2 20
for i in {1..10}; do
    tc g2-$i gen_rand r=5 c=5 p_blocked=0.3 max_coord=200000
done

for i in {11..20}; do
    tc g2-$i gen_rand r=5 c=5 p_blocked=0.4 max_coord=200000
done

for i in {21..30}; do
    tc g2-$i gen_rand r=5 c=5 p_blocked=0.5 max_coord=200000
done

group group3 20
for i in {1..10}; do
    tc g3-$i gen_rand r=5 c=5 p_blocked=0.3 max_coord=1000000000
done

for i in {11..20}; do
    tc g3-$i gen_rand r=5 c=5 p_blocked=0.4 max_coord=1000000000
done

for i in {21..30}; do
    tc g3-$i gen_rand r=5 c=5 p_blocked=0.5 max_coord=1000000000
done
tc_manual ../manual/nils.in
