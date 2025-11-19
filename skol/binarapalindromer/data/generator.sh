#!/usr/bin/env bash

PPATH=$(realpath ..)

REQUIRE_SAMPLE_REUSE=0

. ../../../testdata_tools/gen.sh

#ulimit -s unlimited

use_solution harry.py

compile gen_rand.py
compile gen_edge.py

samplegroup
sample 1
sample 2
sample 3

group group1 20
limits maxn=1000000000
tc_manual ../manual/secret_1.in

for i in {01..05}; do
  tc g1-$i gen_rand a=$((RANDOM * RANDOM % 1000 + 1))  b=$((RANDOM * RANDOM % 1000000 + 1)) 
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
  tc g1-$i gen_edge maxbits=30 shift=1
done

group group2 20
limits maxk=4
tc_manual ../manual/secret_2.in
#tc g1-06
#tc g1-07
for i in {01..05}; do
  tc g2-$i gen_rand a=$((RANDOM + 1)) b=$((1000000000000000000 - RANDOM )) k=$((RANDOM % 4 + 1))
done

tc g2-06 gen_rand a=1 b=1000000000000000000 k=4
tc g2-07 gen_rand a=1 b=1000000000000000000 k=3
tc g2-08 gen_rand a=1 b=1000000000000000000 k=2
tc g2-09 gen_rand a=1 b=1000000000000000000 k=1

for i in {10..18}; do
  tc g2-$i gen_edge maxbits=59 shift=1 k=$(($i % 4 + 1))
done

group group3 20
tc_manual ../manual/secret_3.in

for i in {01..05}; do
  tc g3-$i gen_rand 
done
for i in {06..14}; do
  tc g3-$i gen_edge maxbits=59 shift=1 
done


group group4 20
tc_manual ../manual/secret_4.in

group group5 20
tc_manual ../manual/secret_5.in

tc g3-15 gen_rand a=1 b=1000000000000000000 k=30
tc g3-16 gen_rand a=1 b=1000000000000000000 k=31
tc g3-17 gen_rand a=1 b=1000000000000000000 k=47
tc g3-18 gen_rand a=1 b=1000000000000000000 k=48
tc g3-19 gen_rand a=1 b=1000000000000000000 k=59
tc g3-20 gen_rand a=1 b=1000000000000000000 k=58

