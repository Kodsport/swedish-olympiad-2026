#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution joshua.cpp

compile gen_rand.py

samplegroup
sample 1
sample 2

MAXN=200000

group group1 15
limits maxn=100
include_group sample
tc g1-1 gen_rand n=100 mode=random maxv=1
tc g1-2 gen_rand n=100 mode=random maxv=2
tc g1-3 gen_rand n=100 mode=random maxv=3
tc g1-4 gen_rand n=100 mode=random maxv=100
tc g1-5 gen_rand n=100 mode=almost_inc
tc g1-6 gen_rand n=100 mode=almost_dec

group group2 30
limits maxn=5000
include_group group1
tc g2-1 gen_rand n=5000 mode=random maxv=1
tc g2-2 gen_rand n=5000 mode=random maxv=2
tc g2-3 gen_rand n=5000 mode=random maxv=3
tc g2-4 gen_rand n=5000 mode=random maxv=5000
tc g2-5 gen_rand n=5000 mode=almost_inc
tc g2-6 gen_rand n=5000 mode=almost_dec p_swaps=0.01
tc g2-7 gen_rand n=5000 mode=almost_dec p_swaps=0.1
tc g2-8 gen_rand n=5000 mode=almost_dec p_swaps=0.3
tc g2-9 gen_rand n=5000 mode=almost_dec p_swaps=0.5

group group3 10
limits maxval=2
tc g3-1 gen_rand n=$MAXN mode=random maxv=1
tc g3-2 gen_rand n=$MAXN mode=random maxv=2
tc g3-3 gen_rand n=$MAXN mode=random maxv=2
tc g3-4 gen_rand n=$MAXN mode=random maxv=2

group group4 14
limits maxval=20
include_group group3
tc g4-1 gen_rand n=$MAXN mode=random maxv=10
tc g4-2 gen_rand n=$MAXN mode=random maxv=10
tc g4-3 gen_rand n=$MAXN mode=random maxv=10
tc g4-4 gen_rand n=$MAXN mode=random maxv=10

group group5 31
include_group group2
include_group group4
tc g5-2 gen_rand n=$MAXN mode=random maxv=2
tc g5-3 gen_rand n=$MAXN mode=random maxv=3
tc g5-4 gen_rand n=$MAXN mode=random maxv=10
tc g5-5 gen_rand n=$MAXN mode=random maxv=100
tc g5-6 gen_rand n=$MAXN mode=random maxv=1000
tc g5-7 gen_rand n=$MAXN mode=random maxv=10000
tc g5-8 gen_rand n=$MAXN mode=random maxv=$MAXN
tc g5-9 gen_rand n=$MAXN mode=almost_inc
tc g5-10 gen_rand n=$MAXN mode=almost_dec p_swaps=0.01
tc g5-11 gen_rand n=$MAXN mode=almost_dec p_swaps=0.1
tc g5-12 gen_rand n=$MAXN mode=almost_dec p_swaps=0.3
tc g5-13 gen_rand n=$MAXN mode=almost_dec p_swaps=0.5
tc g5-14 gen_rand n=$MAXN mode=almost_dec p_swaps=0.7
tc g5-15 gen_rand n=$MAXN mode=almost_dec p_swaps=0.9
