#!/bin/bash
. ../../../testdata_tools/gen.sh

use_solution joshua.cpp

compile gen_rand.py

samplegroup
sample 1
sample 2

# Intentionally do not merge groups to see what solutions actually get during skolkval

group group1 20
limits maxn=10 m=3
tc_manual 13
tc_manual 23
tc_manual 33
tc_manual 43
tc_manual 53
tc_manual 63
tc_manual 73
tc_manual 83
tc_manual 93

group group2 20
limits m=2
tc 1
tc_manual ../manual/secret_2.in
tc g2-1 gen_rand n=1000000000 m=2
tc g2-2 gen_rand n=1000000000 m=2
tc g2-3 gen_rand n=1000000000 m=2
tc g2-4 gen_rand n=1000000000 m=2
tc g2-5 gen_rand n=1000000000 m=2

group group3 20
limits maxn=100000
tc_manual ../manual/secret_3.in
tc g3-1 gen_rand n=100000 m=2
tc g3-2 gen_rand n=100000 m=3
tc g3-3 gen_rand n=100000 m=4
tc g3-4 gen_rand n=100000 m=5
tc g3-5 gen_rand n=100000 m=6
tc g3-6 gen_rand n=100000 m=7
tc g3-7 gen_rand n=100000 m=8
tc g3-8 gen_rand n=100000 m=9

group group4 40
include_group group1
include_group group2
include_group group3
tc 2
tc_manual ../manual/secret_4.in
tc_manual ../manual/secret_5.in
tc g4-1 gen_rand n=1000000000 m=2
tc g4-2 gen_rand n=1000000000 m=3
tc g4-3 gen_rand n=1000000000 m=4
tc g4-4 gen_rand n=1000000000 m=5
tc g4-5 gen_rand n=1000000000 m=6
tc g4-6 gen_rand n=1000000000 m=7
tc g4-7 gen_rand n=1000000000 m=8
tc g4-8 gen_rand n=1000000000 m=9
