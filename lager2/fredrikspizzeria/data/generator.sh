#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../testdata_tools/gen.sh

ulimit -s unlimited

use_solution codex.cpp

compile gen_tree.py
compile gen_rand.py
compile gen_rand_vertexcactus.py
compile gen_kill_m2.py

samplegroup
sample 1
sample 2

MAXN=200000
MAXM=200000

N=99
SQRTN=8
group group1 5
limits maxn=100 maxm=100 tree=1
tc g1-1 gen_tree n=$N mode=random
tc g1-2 gen_tree n=$N mode=random
tc g1-3 gen_tree n=$N mode=prufer
tc g1-4 gen_tree n=$N mode=prufer
tc g1-5 gen_tree n=$N mode=star extra-nodes=$SQRTN
tc g1-6 gen_tree n=$N mode=path extra-nodes=$SQRTN
tc g1-7 gen_tree n=$N mode=broomstick extra-nodes=$SQRTN
tc g1-9 gen_tree n=$N mode=dumbbell extra-nodes=$SQRTN
tc g1-10 gen_tree n=$N mode=dumbbell extra-nodes=$SQRTN

N=$((MAXN-1))
SQRTN=300
group group2 10
limits tree=1
include_group group1
tc g2-1 gen_tree n=$N mode=random
tc g2-2 gen_tree n=$N mode=random
tc g2-3 gen_tree n=$N mode=prufer
tc g2-4 gen_tree n=$N mode=prufer
tc g2-5 gen_tree n=$N mode=star extra-nodes=$SQRTN
tc g2-6 gen_tree n=$N mode=path extra-nodes=$SQRTN
tc g2-7 gen_tree n=$N mode=broomstick extra-nodes=$SQRTN
tc g2-9 gen_tree n=$N mode=dumbbell extra-nodes=$SQRTN
tc g2-10 gen_tree n=$N mode=dumbbell extra-nodes=$SQRTN

N=100
M=100
group group3 12
limits maxn=$N maxm=$M
include_group sample group1
tc g3-1 gen_rand n=90 seed=827
tc g3-2 gen_rand n=90 seed=500
tc g3-3 gen_rand n=90

N=2000
M=2000
group group4 8
limits maxn=$N maxm=$M vertexcactus=1
include_group group1
tc 1
tc g4-1 gen_rand_vertexcactus n=1950 cycles=1
tc g4-2 gen_rand_vertexcactus n=1951 cycles=2
tc g4-3 gen_rand_vertexcactus n=1950 cycles=3
tc g4-4 gen_rand_vertexcactus n=1951 cycles=10
tc g4-5 gen_rand_vertexcactus n=1851 cycles=100

N=2000
M=2000
group group5 15
limits maxn=$N maxm=$M
include_group group3 group4
tc g5-1 gen_rand n=1950 seed=902
tc g5-2 gen_rand n=1950

group group6 19
limits vertexcactus=1
include_group group2 group4
tc g6-1 gen_rand_vertexcactus n=199950 cycles=1
tc g6-2 gen_rand_vertexcactus n=199951 cycles=2
tc g6-3 gen_rand_vertexcactus n=199950 cycles=3
tc g6-4 gen_rand_vertexcactus n=199951 cycles=10
tc g6-5 gen_rand_vertexcactus n=199851 cycles=100
tc g6-6 gen_rand_vertexcactus n=198851 cycles=1000
tc g6-7 gen_kill_m2 k=19999

group group7 31
include_group group5 group6
tc g7-1 gen_rand n=199950
tc g7-2 gen_rand n=199950
tc g7-3 gen_kill_m2 k=19999

