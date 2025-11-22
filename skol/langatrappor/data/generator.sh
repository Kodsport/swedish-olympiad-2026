#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution joshua.cpp

compile gen_rand.py

# VERY todo

samplegroup
sample 1

MAXR=1000000000
group group1 20
limits maxc=2
tc_manual ../skolkval_tests/skol_1.in
tc g1-1 gen_rand r=$MAXR c=2 mode=3x
tc g1-2 gen_rand r=$MAXR c=2 mode=3x
tc g1-3 gen_rand r=$MAXR c=2 mode=3x
tc g1-4 gen_rand r=$MAXR c=2 mode=exp base=1.1
tc g1-5 gen_rand r=$MAXR c=2 mode=exp base=1.5
tc g1-6 gen_rand r=$MAXR c=2 mode=exp base=1.7
tc g1-7 gen_rand r=$MAXR c=2 mode=exp base=2
tc g1-8 gen_rand r=$MAXR c=2 mode=exp base=2
tc g1-9 gen_rand r=$MAXR c=2 mode=exp base=3

