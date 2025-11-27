#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

ulimit -s unlimited
use_solution joshua_opt.cpp

compile gen_rand.py

samplegroup
sample 1

MAX=100000
group group1 3
limits maxn=100 maxm=100 maxq=100 mint=2
tc g1-1 gen_rand n=100 m=100 q=100 treemode=errichto lo=0 hi=1 querymode=random mint=2
tc g1-2 gen_rand n=100 m=100 q=100 treemode=errichto lo=0 hi=0.1 querymode=random mint=2
tc g1-3 gen_rand n=100 m=100 q=100 treemode=errichto lo=0.6 hi=0.8 querymode=random mint=2
tc g1-4 gen_rand n=100 m=100 q=100 treemode=broomstick querymode=leafs mint=2
tc g1-5 gen_rand n=100 m=100 q=100 treemode=prufer querymode=random mint=2

group group2 7
limits maxn=100 maxm=100 maxq=100
include_group sample
include_group group1
tc g2-1 gen_rand n=100 m=100 q=100 treemode=errichto lo=0 hi=1 querymode=random
tc g2-2 gen_rand n=100 m=100 q=100 treemode=errichto lo=0 hi=0.1 querymode=random
tc g2-3 gen_rand n=100 m=100 q=100 treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g2-4 gen_rand n=100 m=100 q=100 treemode=broomstick querymode=leafs
tc g2-5 gen_rand n=100 m=100 q=100 treemode=broomstick querymode=leafs mint=2
tc g2-6 gen_rand n=100 m=100 q=100 treemode=prufer querymode=random

group group3 25
limits maxn=2000 maxm=2000 maxq=2000
include_group group2
tc g3-1 gen_rand n=2000 m=2000 q=2000 treemode=errichto lo=0 hi=1 querymode=random
tc g3-2 gen_rand n=2000 m=2000 q=2000 treemode=errichto lo=0 hi=0.1 querymode=random
tc g3-3 gen_rand n=2000 m=2000 q=2000 treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g3-4 gen_rand n=2000 m=2000 q=2000 treemode=broomstick querymode=leafs
tc g3-5 gen_rand n=2000 m=2000 q=2000 treemode=broomstick querymode=leafs mint=2
tc g3-6 gen_rand n=2000 m=2000 q=2000 treemode=prufer querymode=random

group group4 15
tc g4-1 gen_rand n=$MAX m=$MAX q=$MAX treemode=line querymode=random
tc g4-2 gen_rand n=$MAX m=$MAX q=$MAX treemode=line querymode=random mint=2
tc g4-3 gen_rand n=$MAX m=$MAX q=$MAX treemode=line querymode=line
tc g4-4 gen_rand n=$MAX m=$MAX q=$MAX treemode=line querymode=line mint=2

group group5 12
limits maxr=1
tc g5-1 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.01 querymode=random maxr=1
tc g5-2 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.02 querymode=random maxr=1
tc g5-3 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.03 querymode=random maxr=1
tc g5-4 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=1 querymode=random maxr=1
tc g5-5 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.1 querymode=random maxr=1
tc g5-6 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0.6 hi=0.8 querymode=random maxr=1
tc g5-7 gen_rand n=$MAX m=$MAX q=$MAX treemode=broomstick querymode=leafs maxr=1
tc g5-8 gen_rand n=$MAX m=$MAX q=$MAX treemode=broomstick querymode=leafs mint=2 maxr=1
tc g5-9 gen_rand n=$MAX m=$MAX q=$MAX treemode=prufer querymode=random maxr=1
tc g5-10 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.001 querymode=random maxr=1
tc g5-11 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.0001 querymode=random maxr=1
tc g5-12 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.00001 querymode=random maxr=1

group group6 20
limits maxm=100
tc g6-1 gen_rand n=$MAX m=100 q=$MAX treemode=errichto lo=0 hi=1 querymode=random
tc g6-2 gen_rand n=$MAX m=100 q=$MAX treemode=errichto lo=0 hi=0.1 querymode=random
tc g6-3 gen_rand n=$MAX m=100 q=$MAX treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g6-4 gen_rand n=$MAX m=100 q=$MAX treemode=broomstick querymode=leafs
tc g6-5 gen_rand n=$MAX m=100 q=$MAX treemode=broomstick querymode=leafs mint=2
tc g6-6 gen_rand n=$MAX m=100 q=$MAX treemode=prufer querymode=random

group group7 5
limits maxn=25000 maxm=25000 maxq=25000
include_group group3
tc g7-1 gen_rand n=25000 m=25000 q=25000 treemode=errichto lo=0 hi=1 querymode=random
tc g7-2 gen_rand n=25000 m=25000 q=25000 treemode=errichto lo=0 hi=0.1 querymode=random
tc g7-3 gen_rand n=25000 m=25000 q=25000 treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g7-4 gen_rand n=25000 m=25000 q=25000 treemode=broomstick querymode=leafs
tc g7-5 gen_rand n=25000 m=25000 q=25000 treemode=broomstick querymode=leafs mint=2
tc g7-6 gen_rand n=25000 m=25000 q=25000 treemode=prufer querymode=random

group group8 13
include_group group4
include_group group5
include_group group6
include_group group7
tc g8-1 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=1 querymode=random
tc g8-2 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.1 querymode=random
tc g8-3 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g8-4 gen_rand n=$MAX m=$MAX q=$MAX treemode=broomstick querymode=leafs
tc g8-5 gen_rand n=$MAX m=$MAX q=$MAX treemode=broomstick querymode=leafs mint=2
tc g8-6 gen_rand n=$MAX m=$MAX q=$MAX treemode=prufer querymode=random
