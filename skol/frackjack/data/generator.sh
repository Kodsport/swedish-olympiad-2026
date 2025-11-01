#!/bin/bash
. ../../../testdata_tools/gen.sh

use_solution ruiming.cpp

compile gen_rand.py

#samplegroup
#sample_manual 1
#sample_manual 2

# Intentionally do not merge groups to see what solutions actually get during skolkval

group group1 100
limits maxa=30 maxb=30  
#tc_manual ../manual/secret_${i}.in
tc 1
tc 2
tc 3
tc 4
tc 5
tc 6
tc 7
tc 8
tc 9
tc 10
