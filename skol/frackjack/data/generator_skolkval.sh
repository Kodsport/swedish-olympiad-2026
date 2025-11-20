#!/bin/bash
REQUIRE_SAMPLE_REUSE=0
. ../../../testdata_tools/gen.sh

use_solution joshua.py

samplegroup
sample 1
sample 2

group group1 20
limits a_neq_b=1 maxval=21
tc_manual ../manual/secret_1.in

group group2 20
limits maxval=21
tc_manual ../manual/secret_2.in

group group3 20
limits jack_not_win=1
tc_manual ../manual/secret_3.in

group group4 20
tc_manual ../manual/secret_4.in

group group5 20
tc_manual ../manual/secret_5.in
