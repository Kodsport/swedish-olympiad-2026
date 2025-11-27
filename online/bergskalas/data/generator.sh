#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

ulimit -s unlimited
use_solution joshua_opt.cpp

compile gen_rand.py

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
include_group group1
tc g2-1 gen_rand n=100 m=100 q=100 treemode=errichto lo=0 hi=1 querymode=random
tc g2-2 gen_rand n=100 m=100 q=100 treemode=errichto lo=0 hi=0.1 querymode=random
tc g2-3 gen_rand n=100 m=100 q=100 treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g2-4 gen_rand n=100 m=100 q=100 treemode=broomstick querymode=leafs
tc g2-5 gen_rand n=100 m=100 q=100 treemode=broomstick querymode=leafs mint=2
tc g2-6 gen_rand n=100 m=100 q=100 treemode=prufer querymode=random

group group3 25
limits maxn=1000 maxm=1000 maxq=1000
include_group group2
tc g3-1 gen_rand n=1000 m=1000 q=1000 treemode=errichto lo=0 hi=1 querymode=random
tc g3-2 gen_rand n=1000 m=1000 q=1000 treemode=errichto lo=0 hi=0.1 querymode=random
tc g3-3 gen_rand n=1000 m=1000 q=1000 treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g3-4 gen_rand n=1000 m=1000 q=1000 treemode=broomstick querymode=leafs
tc g3-5 gen_rand n=1000 m=1000 q=1000 treemode=broomstick querymode=leafs mint=2
tc g3-6 gen_rand n=1000 m=1000 q=1000 treemode=prufer querymode=random

group group4 15
tc g4-1 gen_rand n=$MAX m=$MAX q=$MAX treemode=line querymode=random
tc g4-2 gen_rand n=$MAX m=$MAX q=$MAX treemode=line querymode=random mint=2

group group7 18
limits maxn=25000 maxm=25000 maxq=25000
include_group group3
tc g7-1 gen_rand n=25000 m=25000 q=25000 treemode=errichto lo=0 hi=1 querymode=random
tc g7-2 gen_rand n=25000 m=25000 q=25000 treemode=errichto lo=0 hi=0.1 querymode=random
tc g7-3 gen_rand n=25000 m=25000 q=25000 treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g7-4 gen_rand n=25000 m=25000 q=25000 treemode=broomstick querymode=leafs
tc g7-5 gen_rand n=25000 m=25000 q=25000 treemode=broomstick querymode=leafs mint=2
tc g7-6 gen_rand n=25000 m=25000 q=25000 treemode=prufer querymode=random

group group8 18
limits maxn=$MAX maxm=$MAX maxq=$MAX
include_group group7
tc g8-1 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=1 querymode=random
tc g8-2 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0 hi=0.1 querymode=random
tc g8-3 gen_rand n=$MAX m=$MAX q=$MAX treemode=errichto lo=0.6 hi=0.8 querymode=random
tc g8-4 gen_rand n=$MAX m=$MAX q=$MAX treemode=broomstick querymode=leafs
tc g8-5 gen_rand n=$MAX m=$MAX q=$MAX treemode=broomstick querymode=leafs mint=2
tc g8-6 gen_rand n=$MAX m=$MAX q=$MAX treemode=prufer querymode=random
