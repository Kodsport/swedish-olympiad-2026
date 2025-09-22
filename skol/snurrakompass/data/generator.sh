#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

ulimit -s unlimited # vad gör detta?

use_solution raunak.cpp

# Möjligen Jobbiga testfall

compile gen_rand.py

samplegroup
sample_manual 1
sample_manual 2

group group1 30
limits ab=1
tc 2
tc g1-1 gen_rand ab=1
tc g1-2 gen_rand ab=1
tc g1-3 gen_rand ab=1
tc g1-4 gen_rand ab=1
tc g1-5 gen_rand ab=1
tc g1-6 gen_rand ab=1
tc g1-7 gen_rand ab=1
tc g1-8 gen_rand ab=1

group group2 70
include_group group1
tc 1
tc g2-1 gen_rand
tc g2-2 gen_rand
tc g2-3 gen_rand
tc g2-4 gen_rand
tc g2-5 gen_rand
tc g2-6 gen_rand 
tc g2-7 gen_rand
tc g2-8 gen_rand
tc g2-9 gen_rand
tc g2-10 gen_rand
