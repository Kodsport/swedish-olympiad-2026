#!/bin/bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution joshua.cpp opt

compile gen_rand.py
compile gen_highpenalty.py
compile gen_adversarial.py
compile gen_nils.py

# TODO intervals cover each other a long time
# TODO brute bug5 and bug6

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
tc g1-6 gen_rand mode=one_sub_per_problem n=50 p=1 h=0 f=50 
tc g1-7 gen_rand mode=one_sub_per_problem n=50 p=1 h=49 f=1 
# Implied: F,H <= N * P
group group2 6
limits maxn=50 once_per_problem=1
include_group group1
tc g2-1 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=0
tc g2-2 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=0.25
tc g2-3 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=0.5
tc g2-4 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=0.75
tc g2-5 gen_rand mode=one_sub_per_problem n=50 p=15 h=25 f=25 p_ac=1
tc g2-6 gen_rand mode=one_sub_per_problem n=50 p=15 h=0 f=50 
tc g2-7 gen_rand mode=one_sub_per_problem n=50 p=15 h=49 f=1 

tc g3-5 gen_rand mode=random n=50 p=15 h=50 f=50 p_ac=1

MAXVAL=50
group group3 15
limits maxn=$MAXVAL maxh=$MAXVAL maxf=$MAXVAL
include_group sample
include_group group2
tc g3-1 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0
tc g3-2 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.25
tc g3-3 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.5
tc g3-4 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.75
tc g3-5 # moved to group2
tc g3-6 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL 
tc g3-7 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=10
tc g3-8 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=20
tc g3-9 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL seed=1
tc_manual ../manual_testcases/n50_1.in
tc_manual ../manual_testcases/n50_2.in
tc_manual ../manual_testcases/n50_3.in

MAXVAL=500
group group4 8
limits maxn=$MAXVAL maxh=$MAXVAL maxf=$MAXVAL
include_group group3
tc g4-1 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0
tc g4-2 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.25
tc g4-3 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.5
tc g4-4 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.75
tc g4-5 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=1
tc g4-6 gen_highpenalty n=$MAXVAL p=3 h=$MAXVAL f=$MAXVAL 
tc g4-7 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=10
tc g4-8 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=250
tc_manual ../manual_testcases/n500_1.in
tc_manual ../manual_testcases/n500_2.in
tc_manual ../manual_testcases/n500_3.in

MAXVAL=2000
group group5 30
limits maxn=$MAXVAL maxh=$MAXVAL maxf=$MAXVAL
include_group group4
tc g5-1 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0
tc g5-2 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.25
tc g5-3 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.5
tc g5-4 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.75
tc g5-5 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=1
tc g5-6 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL 
tc g5-7 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=10
tc g5-8 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=600
tc g5-9 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=1450
tc g5-10 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=12
tc_manual ../manual_testcases/n2000_1.in
tc_manual ../manual_testcases/n2000_2.in
tc_manual ../manual_testcases/n2000_3.in


MAXVAL=200000
group group6 36
include_group group2
include_group group5
tc g6-1 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0
tc g6-2 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.25
tc g6-3 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.5
tc g6-4 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=0.75
tc g6-5 gen_rand mode=random n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL p_ac=1
tc g6-6 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL 
tc g6-7 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=10
tc g6-8 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=66666
tc g6-9 gen_highpenalty n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL prob=420
tc g6-10 gen_rand mode=random n=1000 p=13 h=100 f=$MAXVAL p_ac=0.01
tc g6-11 gen_rand mode=random n=2000 p=15 h=$MAXVAL f=$MAXVAL p_ac=0.01
tc g6-12 gen_rand mode=random n=500 p=15 h=20000 f=$MAXVAL p_ac=0.007
tc g6-13 gen_rand mode=random n=500 p=15 h=$MAXVAL f=$MAXVAL p_ac=0.001
tc_manual ../manual_testcases/n200000_1.in
tc g6-nils-1 gen_nils n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL
tc g6-nils-2 gen_nils n=$MAXVAL p=15 h=0 f=$MAXVAL
tc g6-nils-3 gen_nils n=$MAXVAL p=15 h=$MAXVAL f=$MAXVAL shuffle=1
tc g6-nils-4 gen_nils n=$MAXVAL p=15 h=0 f=$MAXVAL shuffle=1
