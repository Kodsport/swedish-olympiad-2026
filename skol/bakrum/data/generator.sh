#!/bin/bash
. ../../../testdata_tools/gen.sh

use_solution ../z3_chatgpt.py cpython3

compile gen_rand.py

samplegroup
sample 1

gen_rand_cases() {
    for i in {1..10}; do
        tc g$1-$i gen_rand r=$2 c=$3 p_blocked=0.3 max_coord=$4
    done

    for i in {11..20}; do
        tc g$1-$i gen_rand r=$2 c=$3 p_blocked=0.4 max_coord=$4
    done

    for i in {21..30}; do
        tc g$1-$i gen_rand r=$2 c=$3 p_blocked=0.5 max_coord=$4
    done
}

group group1 20
limits maxr=2
tg_manual ../manual_tests/2x2_bigcoord
gen_rand_cases 1 2 2 1000000000

group group2 20
limits max_coord=500
tg_manual ../manual_tests/2x2_smallcoord
gen_rand_cases 2 5 5 500

group group3 20
limits max_coord=100000
include_group group2
gen_rand_cases 3 5 5 100000

group group4 40
include_group sample
include_group group1
include_group group3
gen_rand_cases 4 5 5 1000000000
