#!/bin/bash
REQUIRE_SAMPLE_REUSE=0
. ../../../testdata_tools/gen.sh

use_solution nils.py

samplegroup
sample 1
sample 2
sample 3

group group1 20
limits maxn=1
tc_manual ../skolkval_tests/skol_1.in

group group2 20
tc_manual ../skolkval_tests/skol_2.in

group group3 20
tc_manual ../skolkval_tests/skol_3.in

group group4 20
tc_manual ../skolkval_tests/skol_4.in

group group5 20
tc_manual ../skolkval_tests/skol_5.in
