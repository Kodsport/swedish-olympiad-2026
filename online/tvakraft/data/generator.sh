#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution harry.py

compile gen_rand.py

samplegroup
sample 1
sample 2
sample 3
sample 4

group group1 20
limits maxn=100
tc 1
tc 2
tc g1-01 gen_rand N=4
tc g1-02 gen_rand N=8
tc g1-03 gen_rand N=7
tc g1-04 gen_rand N=16
tc g1-05 gen_rand N=33
tc g1-06 gen_rand N=100
tc g1-07 gen_rand N=32
tc g1-08 gen_rand N=64
tc g1-09 gen_rand N=99
tc g1-10 gen_rand N=66

group group2 80

include_group sample
include_group group1
tc g2-01 gen_rand N=128
tc g2-02 gen_rand N=1024
tc g2-03 gen_rand N=10240000
tc g2-04 gen_rand N=10240000000000
tc g2-05 gen_rand N=256
tc g2-06 gen_rand N=512
tc g2-07 gen_rand N=1000000000000000
tc g2-08 gen_rand N=10000000000000000
tc g2-09 gen_rand N=1000000000000000000
tc g2-10 gen_rand N=2048
tc g2-11 gen_rand N=4096
tc g2-12 gen_rand N=8192
tc g2-13 gen_rand N=16384
tc g2-14 gen_rand N=32768
tc g2-15 gen_rand N=65536
tc g2-16 gen_rand N=131072
tc g2-17 gen_rand N=1048576
tc g2-18 gen_rand N=16777216
tc g2-19 gen_rand N=1073741824
tc g2-20 gen_rand N=1099511627776
tc g2-21 gen_rand N=1125899906842624
tc g2-22 gen_rand N=115292154606846976
tc g2-23 gen_rand N=576460752303423488
tc g2-24 gen_rand N=576460752303423489
tc g2-25 gen_rand N=576460752303423490
tc g2-26 gen_rand N=576460752303423487
