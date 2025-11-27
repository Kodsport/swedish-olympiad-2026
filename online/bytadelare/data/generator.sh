#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

#ulimit -s unlimited

use_solution nils.cpp

compile gen.py

samplegroup
sample_manual 1
sample_manual 2
sample_manual 3

MAXN=1000

group group1 17
limits maxn=10
tc 1
tc 2
tc g1-1 gen n=1
tc g1-2 gen n=4 maxx=4
tc g1-3 gen n=5 maxx=4
tc g1-4 gen n=10 maxx=4
tc g1-5 gen n=10 maxx=10
tc g1-6 gen n=10 maxx=5
tc g1-prime-power-1 gen n=10 maxx=10 mode=prime_power
tc g1-prime-power-2 gen n=10 mode=prime_power
tc g1-prime-forest-1 gen n=10 mode=prime_forest comps=3

group group2 10
limits max_unique=2
tc g2-1 gen n=$MAXN u=2 maxx=2
tc g2-2 gen n=$MAXN u=2 maxx=4
tc g2-3 gen n=$MAXN u=1
tc g2-4 gen n=$MAXN u=2 mode=one_edge

group group3 13
limits max_unique=3
include_group group2
tc g3-1 gen n=$MAXN u=3 maxx=4
tc g3-2 gen n=$MAXN u=3 mode=one_edge
tc g3-3 gen n=$MAXN u=3 mode=one_edge noise=10
tc g3-4 gen n=$MAXN u=3 mode=two_edge

group group4 30
limits maxx=0
tc 3
tc g4-1 gen n=$MAXN x=0 maxx=5
tc g4-2 gen n=$MAXN x=0 maxx=55
tc g4-3 gen n=$MAXN x=0 maxx=555
tc g4-4 gen n=$MAXN x=0 maxx=2000
tc g4-5 gen n=$MAXN x=0 maxx=10000
tc g4-prime-power-1 gen n=$MAXN x=0 mode=prime_power
tc g4-prime-power-2 gen n=$MAXN x=0 mode=prime_power noise=10
tc g4-prime-forest-1 gen n=$MAXN x=0 mode=prime_forest comps=100
tc g4-prime-forest-2 gen n=$MAXN x=0 mode=prime_forest noise=10 comps=100
tc g4-prime-line-1 gen n=$MAXN  x=0 mode=prime_line
tc g4-prime-line-2 gen n=$MAXN  x=0 mode=prime_line noise=10

group group5 12
limits a1=1
tc g5-1 gen n=$MAXN maxx=55 a1=1
tc g5-2 gen n=$MAXN a1=1

group group6 18
include_group group1
include_group group3
include_group group4
include_group group5

tc g6-1 gen n=$MAXN maxx=111 
tc g6-2 gen n=$MAXN maxx=111111
tc g6-prime-power-1 gen n=$MAXN maxx=1000 mode=prime_power
tc g6-prime-power-2 gen n=$MAXN mode=prime_power
tc g6-prime-forest-1 gen n=$MAXN mode=prime_forest comps=1
tc g6-prime-forest-2 gen n=$MAXN mode=prime_forest comps=10
tc g6-prime-forest-3 gen n=$MAXN mode=prime_forest comps=30
tc g6-prime-forest-4 gen n=$MAXN mode=prime_forest comps=100
tc g6-prime-forest-5 gen n=$MAXN mode=prime_forest comps=999
tc g6-prime-forest-6 gen n=$MAXN mode=prime_forest comps=1 branch=1
tc g6-prime-forest-7 gen n=$MAXN mode=prime_forest comps=2 branch=6
tc g6-prime-line-1 gen n=$MAXN  mode=prime_line

