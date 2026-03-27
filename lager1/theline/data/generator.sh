#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../testdata_tools/gen.sh

#ulimit -s unlimited

use_solution gemini_k2.cpp

compile gen_rand.py
compile gen_anti_nk.py

samplegroup
sample 1
sample 2
sample 3

MAXN=1000000000 # Billion
MAXK=10000

N=50
K=50
group group1 7
limits maxn=$N maxk=$K
include_group sample
for n in {1..50}; do
    tc g1-n$n gen_rand n=$n mode=random
done
tc g1-1 gen_rand n=$N mode=long_intervals k=$K

N=500
K=500
group group2 14
limits maxn=$N maxk=$K
include_group group1
for n in {100..150}; do # More stresstesting just to be safe
    tc g2-n$n gen_rand n=$n mode=random
done
tc g2-1 gen_rand n=$N mode=random k=50
tc g2-2 gen_rand n=$N mode=random k=250
tc g2-3 gen_rand n=$N mode=random k=500
tc g2-4 gen_rand n=$N mode=long_intervals k=$K

N=2000
K=2000
group group3 27
limits maxn=$N maxk=$K
include_group group2
tc g3-1 gen_rand n=$N mode=random k=1000
tc g3-2 gen_rand n=$N mode=random k=1500
tc g3-3 gen_rand n=$N mode=random k=2000
tc g3-4 gen_rand n=$N mode=long_intervals k=$K
tc g3-5 gen_rand n=$N mode=decoys decoys=510 k=$K
tc g3-6 gen_rand n=$N mode=bimodal k=500 d1=20 d2=200 c1=450
tc g3-7 gen_rand n=$N k=1999 mode=distinct
tc g3-8 gen_rand n=$N k=$K mode=concentrated g1=10 g2=200
tc g3-9 gen_rand n=$N k=$K mode=concentrated g1=200 g2=10
tc g3-10 gen_rand n=$N k=$K mode=concentrated g1=10 g2=50
tc g3-anti-nk-1 gen_anti_nk n=$N k=1000 mode=distinct_wide
tc g3-anti-nk-2 gen_anti_nk n=$N k=1000 mode=clustered_wide
tc g3-anti-nk-3 gen_anti_nk n=$N k=1000 mode=spread_long

N=200000
K=500
group group4 13
limits maxn=$N maxk=$K
include_group group2
tc g4-1 gen_rand n=$N mode=random k=$K
tc g4-2 gen_rand n=$N mode=random k=$K
tc g4-3 gen_rand n=$N mode=long_intervals k=$K
tc g4-4 gen_rand n=$N mode=bimodal k=$K d1=100 d2=10000 c1=495
tc g4-5 gen_rand n=$N k=$K mode=distinct

N=200000
K=2000
group group5 18
limits maxn=$N maxk=$K
include_group group3
include_group group4
tc g5-1 gen_rand n=$N mode=random k=$K
tc g5-2 gen_rand n=$N mode=random k=$K
tc g5-3 gen_rand n=$N mode=long_intervals k=$K
tc g5-4 gen_rand n=$N mode=decoys decoys=510 k=$K
tc g5-5 gen_rand n=$N mode=bimodal k=$K d1=100 d2=10000 c1=1970
tc g5-6 gen_rand n=$N k=$K mode=distinct
tc g5-7 gen_rand n=$N k=$K mode=concentrated g1=10 g2=200
tc g5-8 gen_rand n=$N k=$K mode=concentrated g1=200 g2=10
tc g5-9 gen_rand n=$N k=$K mode=concentrated g1=10 g2=50
tc g5-anti-nk-1 gen_anti_nk n=$N k=$K mode=distinct_wide
tc g5-anti-nk-2 gen_anti_nk n=$N k=$K mode=clustered_wide
tc g5-anti-nk-3 gen_anti_nk n=$N k=$K mode=spread_long

group group6 21
include_group group5
tc g6-1 gen_rand n=$MAXN mode=random k=$MAXK
tc g6-2 gen_rand n=$MAXN mode=random k=$MAXK
tc g6-3 gen_rand n=$MAXN mode=long_intervals k=$MAXK
tc g6-4 gen_rand n=$MAXN mode=decoys decoys=2000 k=$MAXK
tc g6-5 gen_rand n=$MAXN mode=bimodal k=$MAXK d1=100 d2=10000 c1=9950
tc g6-6 gen_rand n=$MAXN k=$MAXK mode=distinct
tc g6-7 gen_rand n=$MAXN k=$MAXK mode=concentrated g1=10 g2=2000
tc g6-8 gen_rand n=$MAXN k=$MAXK mode=concentrated g1=2000 g2=10
tc g6-9 gen_rand n=$MAXN k=$MAXK mode=concentrated g1=10 g2=50
tc g6-10 gen_rand n=$MAXN k=$MAXK mode=concentrated g1=10 g2=50
tc g6-11 gen_rand n=$MAXN k=$MAXK mode=concentrated g1=100 g2=100
