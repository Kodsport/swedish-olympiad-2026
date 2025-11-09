#!/bin/bash
. ../../../testdata_tools/gen.sh

use_solution ruiming.cpp

compile gen_rand.py

samplegroup
sample 1
sample 2

# Intentionally do not merge groups to see what solutions actually get during skolkval

group group1 100
limits maxa=30 maxb=30  
#tc_manual ../manual/secret_${i}.in
tc 1
tc 2
tc_manual ../manual/secret_1.in
tc_manual ../manual/secret_2.in
tc_manual ../manual/secret_3.in
tc_manual ../manual/secret_4.in
tc_manual ../manual/secret_5.in
tc g-1 gen_rand a=5 b=5
tc g-2 gen_rand a=5 b=5
tc g-3 gen_rand a=10 b=10
tc g-4 gen_rand a=10 b=10
tc g-5 gen_rand a=10 b=10
tc g-6 gen_rand a=30 b=30
tc g-7 gen_rand a=30 b=30
tc g-8 gen_rand a=30 b=30
tc g-9 gen_rand a=30 b=30
tc g-10 gen_rand a=30 b=30
