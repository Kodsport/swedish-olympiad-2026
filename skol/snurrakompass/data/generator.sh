#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution raunak.cpp

# Möjligen Jobbiga testfall

compile gen_rand.py

samplegroup
sample 1
sample 2

group group1 20
limits prime=1
tc g1-1 gen_rand prime=1
tc g1-2 gen_rand prime=1
tc g1-3 gen_rand prime=1
tc g1-4 gen_rand prime=1
tc g1-5 gen_rand prime=1
tc g1-6 gen_rand prime=1
tc_manual ../manual/skol_1.in

group group2 40
limits ab=1
tc 2
tc g2-1 gen_rand ab=1
tc g2-2 gen_rand ab=1
tc g2-3 gen_rand ab=1
tc g2-4 gen_rand ab=1
tc g2-5 gen_rand ab=1
tc g2-6 gen_rand ab=1
tc g2-7 gen_rand ab=1
tc_manual ../manual/skol_2.in
tc_manual ../manual/skol_3.in

group group3 40
include_group group1
include_group sample
tc g3-1 gen_rand
tc g3-2 gen_rand
tc g3-3 gen_rand
tc g3-4 gen_rand
tc g3-5 gen_rand
tc g3-6 gen_rand 
tc g3-7 gen_rand
tc g3-8 gen_rand
tc g3-9 gen_rand
tc g3-10 gen_rand
tc_manual ../manual/skol_4.in
tc_manual ../manual/skol_5.in
