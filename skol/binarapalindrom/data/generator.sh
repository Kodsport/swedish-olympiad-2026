#!/usr/bin/env bash

. ../../../testdata_tools/gen.sh

use_solution harry.py

compile gen_rand.py
compile gen_edge.py
compile gen_edge2.py


samplegroup
sample 1
sample 2
sample 3

group group1 20
limits maxn=1000000000
include_group sample
tc_manual ../skolkval_tests/skol_1.in

for i in {01..05}; do
  tc g1-$i gen_rand maxa=1000 maxb=1000000
done
tc g1-06 gen_rand a=297 b=297 k=4 
tc g1-07 gen_rand a=297 b=297 k=3 
tc g1-08 gen_rand a=66433 b=66433 k=5 
tc g1-09 gen_rand a=387506973 b=387506974 k=15
tc g1-10 gen_rand a=387506972 b=387506972 k=15
tc g1-11 gen_rand a=387506974 b=387506974 k=15
tc g1-12 gen_rand a=1 b=1000000000 k=12
tc g1-13 gen_rand a=1 b=1000000000 k=17
for i in {14..20}; do
  tc g1-$i gen_edge maxbits=27 shift=1
done
tc g1-21 gen_edge2 shift=1 k=20 maxbits=28
tc g1-22 gen_edge2 shift=1 k=9 maxbits=30 seed=5

group group2 20
limits maxk=4
tc_manual ../skolkval_tests/skol_2.in
tc g1-06
tc g1-07
for i in {01..05}; do
  tc g2-$i gen_rand maxa=32768 minb=999999999999967232 k=$((10#$i % 4 + 1))
done

tc g2-06 gen_rand a=1 b=1000000000000000000 k=4
tc g2-07 gen_rand a=1 b=1000000000000000000 k=3
tc g2-08 gen_rand a=1 b=1000000000000000000 k=2
tc g2-09 gen_rand a=1 b=1000000000000000000 k=1

for i in {10..12}; do
  tc g2-$i gen_edge maxbits=59 shift=1 k=$(($i % 4 + 1))
done

for i in {13..19}; do
  tc g2-$i gen_edge2 shift=1 k=$(($i % 4 + 1))
done
tc g2-20 gen_edge2 shift=1 k=3 seed=2

group group3 60
include_group group1 group2
tc_manual ../skolkval_tests/skol_3.in
tc_manual ../skolkval_tests/skol_4.in
tc_manual ../skolkval_tests/skol_5.in

for i in {01..05}; do
  tc g3-$i gen_rand 
done
for i in {06..14}; do
  tc g3-$i gen_edge maxbits=59 shift=1 
done

tc g3-15 gen_rand a=1 b=1000000000000000000 k=30
tc g3-16 gen_rand a=1 b=1000000000000000000 k=31
tc g3-17 gen_rand a=1 b=1000000000000000000 k=47
tc g3-18 gen_rand a=1 b=1000000000000000000 k=48
tc g3-19 gen_rand a=1 b=1000000000000000000 k=59
tc g3-20 gen_rand a=1 b=1000000000000000000 k=58

tc g3-21 gen_edge2 shift=1 k=35
tc g3-22 gen_edge2 shift=1 k=36
