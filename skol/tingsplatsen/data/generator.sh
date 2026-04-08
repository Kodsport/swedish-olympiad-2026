#!/bin/bash
. ../../../testdata_tools/gen.sh

use_solution nils.py

compile gen_rand.py

samplegroup
sample 1
sample 2
sample 3

group group1 31
limits maxn=1
tc 2
tc_manual ../skolkval_tests/skol_1.in
tc line-1 gen_rand n=1 m=8 p=2

group group2 21
limits rectangle=1
tc_manual ../skolkval_tests/skol_2.in

group group3 48
include_group group1
include_group group2
tc 1
tc 3
tc_manual ../skolkval_tests/skol_3.in
tc_manual ../skolkval_tests/skol_4.in
tc_manual ../skolkval_tests/skol_5.in
tc g2-1 gen_rand n=10 m=10 p=2
tc g2-2 gen_rand n=10 m=10 p=2
tc g2-3 gen_rand n=10 m=10 p=2
tc g2-4 gen_rand n=10 m=10 p=3
tc g2-5 gen_rand n=10 m=10 p=3
tc g2-6 gen_rand n=10 m=10 p=3
tc g2-7 gen_rand n=2 m=10 p=4
tc g2-8 gen_rand n=10 m=10 p=5
tc g2-9 gen_rand n=10 m=10 p=6
tc g2-10 gen_rand n=10 m=10 p=10
tc g2-11 gen_rand n=10 m=10 p=40
tc g2-12 gen_rand n=10 m=10 p=40
