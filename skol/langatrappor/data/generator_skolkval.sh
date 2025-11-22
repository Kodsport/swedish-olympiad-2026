#!/bin/bash
REQUIRE_SAMPLE_REUSE=0
. ../../../testdata_tools/gen.sh

use_solution joshua.cpp

samplegroup
sample 1
sample 2
sample 3

group group1 20
limits c_is=2
tc_manual ../skolkval_tests/skol_1.in

group group2 20
limits maxr=10000
tc_manual ../skolkval_tests/skol_2.in

group group3 20
limits maxr=10000
tc_manual ../skolkval_tests/skol_3.in

group group4 20
tc_manual ../skolkval_tests/skol_4.in

group group5 20
tc_manual ../skolkval_tests/skol_5.in
