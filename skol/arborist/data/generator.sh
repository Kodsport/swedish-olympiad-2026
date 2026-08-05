#!/usr/bin/env bash

. ../../../testdata_tools/gen.sh

use_solution joshua_greedy.py

compile gen_rand.py

samplegroup
sample 1
sample 2

group group1 20
limits k_25=1
tc_manual ../skolkval_tests/skol_1.in
for i in {1..15}; do
    tc g1-$i gen_rand k=25 n=8
done

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


group group3 20
limits same_coord=1
tc_manual ../skolkval_tests/skol_3.in
for i in {1..10}; do
    tc g3-$i gen_rand k=20 n=8 same_coord=1
done

for i in {11..20}; do
    tc g3-$i gen_rand k=40 n=8 same_coord=1
done

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

