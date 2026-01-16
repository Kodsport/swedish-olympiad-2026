#!/bin/bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution joshua.cpp opt

compile gen_rand.py
compile gen_highpenalty.py
compile gen_adversarial.py

# Samples
samplegroup
limits maxn=10 maxp=10 maxf=10 maxh=10
sample 1
sample 2
sample 3
sample 4
sample 5

# Implied: F,H <= N
group group1 5
limits maxn=50 maxp=1 once_per_problem=1
tc 1
tc g1-1 gen_rand mode=one_sub_per_problem n=50 p=1 h=25 f=25 p_ac=0
tc g1-2 gen_rand mode=one_sub_per_problem n=50 p=1 h=25 f=25 p_ac=0.25
tc g1-3 gen_rand mode=one_sub_per_problem n=50 p=1 h=25 f=25 p_ac=0.5
tc g1-4 gen_rand mode=one_sub_per_problem n=50 p=1 h=25 f=25 p_ac=0.75
tc g1-5 gen_rand mode=one_sub_per_problem n=50 p=1 h=25 f=25 p_ac=1

# Implied: F,H <= N * P
group group2 6
limits maxn=50 once_per_problem=1
include_group group1
tc g2-1 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=0
tc g2-2 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=0.25
tc g2-3 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=0.5
tc g2-4 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=0.75
tc g2-5 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=1

MAXVAL=50
group group3 15
limits maxn=$MAXVAL maxh=$MAXVAL maxf=$MAXVAL
include_group sample
tc g3-1 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0
tc g3-2 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.25
tc g3-3 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.5
tc g3-4 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.75
tc g3-5 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=1
#tc g3-6 gen_h


MAXVAL=500
group group4 8
limits maxn=$MAXVAL maxh=$MAXVAL maxf=$MAXVAL
include_group group3
tc g4-1 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0
tc g4-2 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.25
tc g4-3 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.5
tc g4-4 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.75
tc g4-5 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=1

MAXVAL=5000
group group5 30
limits maxn=$MAXVAL maxh=$MAXVAL maxf=$MAXVAL
include_group group4
tc g5-1 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0
tc g5-2 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.25
tc g5-3 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.5
tc g5-4 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.75
tc g5-5 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=1

MAXVAL=200000
group group6 36
include_group group2
include_group group5
tc g6-1 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0
tc g6-2 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.25
tc g6-3 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.5
tc g6-4 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.75
tc g6-5 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=1
