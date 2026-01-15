#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

ulimit -s unlimited

use_solution harry_n.py

compile gen_rand.py


samplegroup
sample 1
sample 2
sample 3

group g1 15
limits maxn=2
tc 1
tc 2
tc g1-1 gen_rand n=2 s=80
tc g1-2 gen_rand n=2 s=17
tc g1-3 gen_rand n=2 s=100 maxt=50 mint=50
tc g1-4 gen_rand n=2 s=999999 maxt=10
tc g1-5 gen_rand n=2
tc g1-6 gen_rand n=2 maxt=1
tc g1-7 gen_rand n=2 s=1235 mint=1235 maxt=1236

group g2 19
limits maxt=1
tc g1-6 
tc g2-1 gen_rand n=3 s=100 maxt=1
tc g2-2 gen_rand n=100 s=1000 maxt=1
tc g2-3 gen_rand n=1000 s=16001 maxt=1
tc g2-4 gen_rand n=200000 s=2000000 maxt=1
tc g2-5 gen_rand n=200000 maxt=1


group g3 27
limits maxn=1000
include_group sample
include_group g1
tc g2-1
tc g2-2
tc g2-3
tc g3-1 gen_rand n=3 s=100 maxt=50 mint=50
tc g3-2 gen_rand n=10 s=1000 maxt=100 
tc g3-3 gen_rand n=30 s=1000 maxt=2000 mint=900
tc g3-4 gen_rand n=1000 maxt=1000
tc g3-5 gen_rand n=1000 maxt=1000000000


group g4 39
include_group g2
include_group g3
tc g4-1 gen_rand n=200000 mint=100000000
tc g4-2 gen_rand n=200000 mint=10000000
tc g4-3 gen_rand n=200000 maxt=10000000
