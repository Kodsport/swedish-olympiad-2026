#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

ulimit -s unlimited 

use_solution harry.py

compile gen_rand.py
compile gen_uniform.py

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

for i in {1..5}; do
    tc_manual ../manual/n8_${i}.in
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
tc g5-1 gen_rand n=100
tc g5-2 gen_rand n=500
tc g5-3 gen_rand n=2000
tc g5-4 gen_rand n=2000 k=2000
tc g5-5 gen_rand n=2000 k=1234

for i in {1..4}; do
    tc_manual ../manual/n2000_${i}.in
done

group group6 24
include_group group2
include_group group3
include_group group5
tc g6-1 gen_rand n=40000
tc g6-2 gen_rand n=200000 k=40000
tc g6-3 gen_rand n=200000 k=123456
tc g6-4 gen_rand n=200000 k=11
tc g6-5 gen_rand n=200000 k=200000

for i in {1..6}; do
    tc_manual ../manual/n2e5_${i}.in
done