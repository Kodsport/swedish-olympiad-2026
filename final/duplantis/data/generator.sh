#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

#ulimit -s unlimited

use_solution nils.py

compile gen.py

samplegroup
sample_manual 1
sample_manual 2

MAXN=300000
MAXT=1000000000

group group1 20
limits sub1=1
tc g1-1 gen n=1 mode=sub1
tc g1-2 gen n=$MAXN mode=sub1
tc g1-3 gen n=$MAXN mode=sub1
tc g1-4 gen n=$MAXN mode=sub1
tc g1-5 gen n=$MAXN mode=sub1 amax=$MAXN
tc g1-6 gen n=$MAXN mode=sub1 amax=1000000
tc g3-1 gen mode=sub3

group group2 20
limits sub2=1
tc 2
tc g1-1
tc g2-1 gen n=1 mode=sub2
tc g2-2 gen n=$MAXN mode=sub2
tc g2-3 gen n=$MAXN mode=sub2
tc g2-4 gen n=$MAXN mode=sub2
tc g2-5 gen n=$MAXN mode=sub2 amax=$MAXN
tc g2-6 gen n=$MAXN mode=sub2 amax=$(($MAXN * 2))

group group3 17
limits maxn=2 minn=2
tc g3-1
tc g3-2 gen mode=sub3
tc g3-3 gen mode=sub3 a1=10 a2=10 v=10
tc g3-4 gen mode=sub3 a1=10 a2=10 v=9
tc g3-5 gen mode=sub3 a1=10 a2=10 v=8
tc g3-6 gen mode=sub3 a1=12 a2=11 v=9
tc g3-7 gen mode=sub3 a1=10 a2=11 v=9
tc g3-8 gen mode=sub3
tc g3-9 gen mode=sub3

group group4 43
include_group group1
include_group group2
include_group group3
tc 1
tc g4-1 gen n=$MAXN
tc g4-2 gen n=$MAXN
tc g4-3 gen n=$MAXN
tc g4-4 gen n=$MAXN
tc g4-5 gen n=$MAXN amax=100000
tc g4-6 gen n=$MAXN amax=10000
tc g4-7 gen n=$MAXN amax=100
tc g4-8 gen n=$MAXN amax=5
tc simulate-1 gen n=$MAXN mode=simulate
tc simulate-2 gen n=$MAXN mode=simulate p=0.99
tc simulate-3 gen n=$MAXN mode=simulate amin=100000 amax=200000
tc simulate-4 gen n=$MAXN mode=simulate p=1.0
tc same-1 gen n=$MAXN amin=$MAXT amax=$MAXT v=$MAXT
tc same-2 gen n=$MAXN amin=$MAXT amax=$MAXT v=$(($MAXT-1))
tc same-3 gen n=$MAXN amin=$MAXT amax=$MAXT v=0
tc same-4 gen n=$MAXN amin=33333333 amax=33333333 v=$MAXT

