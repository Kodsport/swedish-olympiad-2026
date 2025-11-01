#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

ulimit -s unlimited 

use_solution harry.py

compile gen_rand.py
compile gen_uniform.py
compile gen_spike.py

samplegroup
sample_manual 1
sample_manual 2
sample_manual 3

group group1 20
tc_manual affisch

group group2 7
limits maxk=2
tc 2
tc g2-1 gen_rand n=3 k=2
tc g2-2 gen_rand n=8 k=2
tc g2-3 gen_rand n=2000 k=2
tc g2-4 gen_uniform n=8 k=2
tc g2-5 gen_uniform n=2000 k=2
tc g2-6 gen_uniform n=200000 k=2
tc g2-7 gen_rand n=200000 k=2

group group3 9
limits allsame=1
tc g3-1 gen_uniform n=3 k=3
tc g3-2 gen_uniform n=8 k=8
tc g3-3 gen_uniform n=2000 k=1000
tc g3-4 gen_uniform n=2000 k=250
tc g3-5 gen_uniform n=200000 k=25000
tc g3-6 gen_uniform n=200000 k=200000
tc g3-7 gen_uniform n=200000 k=100000
tc g3-8 gen_uniform n=200000 k=10
tc g2-4
tc g2-5
tc g2-6

group group4 12
limits maxn=8
tc 3
tc g2-1
tc g2-2
tc g2-4
tc g3-1
tc g3-2
tc g4-1 gen_rand n=3
tc g4-2 gen_rand n=5
tc g4-3 gen_rand n=8
tc g4-4 gen_spike n=8 k=3 a=4
tc g4-5 gen_spike n=8 k=4 a=5
tc g4-6 gen_spike n=7 k=3 a=4

for i in {1..5}; do
    tc_manual ../manual/n8_${i}.in
done

for i in {1..6}; do
    tc_manual ../manual/n8_p${i}.in
done

group group5 28
limits maxn=2000
include_group sample
include_group group1
include_group group4
tc g2-3
tc g2-5
tc g3-3
tc g3-4 
tc g5-01 gen_rand n=100
tc g5-02 gen_rand n=500
tc g5-03 gen_rand n=2000
tc g5-04 gen_rand n=2000 k=2000
tc g5-05 gen_rand n=2000 k=1234
tc g5-06 gen_spike n=100 k=25 a=50
tc g5-07 gen_spike n=2000 k=399 a=1000
tc g5-08 gen_spike n=1999 k=399 a=1000
tc g5-09 gen_spike n=2000 k=3 a=1000 b=999
tc g5-10 gen_spike n=2000 k=927 a=1001

for i in {1..5}; do
    tc_manual ../manual/n2000_${i}.in
done
for i in {1..6}; do
    tc_manual ../manual/n2000_p${i}.in
done

group group6 24
include_group group2
include_group group3
include_group group5
tc g6-01 gen_rand n=40000
tc g6-02 gen_rand n=200000 k=40000
tc g6-03 gen_rand n=200000 k=123456
tc g6-04 gen_rand n=200000 k=11
tc g6-05 gen_rand n=200000 k=200000
tc g6-06 gen_spike n=200000 k=3 a=100000 b=99999
tc g6-07 gen_spike n=200000 k=15815 a=100000
tc g6-08 gen_spike n=199999 k=25815 a=100000
tc g6-09 gen_spike n=200000 k=99815 a=100001 b=100
tc g6-10 gen_spike n=200000 k=97815 a=100005 b=1000

for i in {1..6}; do
    tc_manual ../manual/n2e5_${i}.in
done

for i in {1..6}; do
    tc_manual ../manual/n2e5_p${i}.in
done