#!/bin/bash
REQUIRE_SAMPLE_REUSE=0
. ../../../testdata_tools/gen.sh

use_solution ../z3_chatgpt.py cpython3

samplegroup
sample 1

group group1 20
limits maxr=2
tg_manual ../skolkval_tests/1_2x2.in

group group2 20
limits max_coord=500
tg_manual ../skolkval_tests/2.in

group group3 20
limits max_coord=100000
tg_manual ../skolkval_tests/3.in

group group4 20
tg_manual ../skolkval_tests/4.in

group group5 20
tg_manual ../skolkval_tests/5.in
