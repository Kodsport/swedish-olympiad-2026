#!/bin/bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution joshua_n2f2.cpp

compile gen_rand.py

samplegroup
limits maxn=10 maxp=10 maxf=10 maxh=10
sample 1
sample 2
sample 3
sample 4
sample 5

M=50
group group2 40
limits maxn=50 maxh=50 maxf=50 maxp=15
for i in {1..10}; do
    tc g1-$i gen_rand n=$M p=15 h=$M f=$M
done
