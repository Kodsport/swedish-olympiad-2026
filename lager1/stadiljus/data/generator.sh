#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../testdata_tools/gen.sh

# ulimit -s unlimited

use_solution GPT.py

compile gen_rand.py
compile gen_perm.py

samplegroup
sample 1
sample 2
sample 3

group group1 8
limits maxn=15
include_group sample
tc g1-rand-06 gen_rand n=6
tc g1-rand-10 gen_rand n=10
tc g1-rand-15 gen_rand n=15
tc g1-inc gen_perm n=15 mode=inc
tc g1-dec gen_perm n=15 mode=dec
tc g1-alt gen_perm n=15 mode=alternating
tc g1-bitonic gen_perm n=15 mode=bitonic
tc g1-rec gen_perm n=15 mode=recursive
tc g1-blocks gen_perm n=15 mode=blocks block=4

group group2 12
limits maxn=200
include_group group1
tc g2-rand-50 gen_rand n=50
tc g2-rand-199 gen_rand n=199
tc g2-rand-200 gen_rand n=200
tc g2-inc gen_perm n=200 mode=inc
tc g2-dec gen_perm n=200 mode=dec
tc g2-alt gen_perm n=200 mode=alternating
tc g2-bitonic gen_perm n=200 mode=bitonic
tc g2-rec gen_perm n=200 mode=recursive
tc g2-blocks-7 gen_perm n=200 mode=blocks block=7
tc g2-blocks-28 gen_perm n=200 mode=blocks block=28

group group3 17
limits maxn=1000
include_group group2
tc g3-rand-200 gen_rand n=200
tc g3-rand-999 gen_rand n=999
tc g3-rand-1000 gen_rand n=1000
tc g3-inc gen_perm n=1000 mode=inc
tc g3-dec gen_perm n=1000 mode=dec
tc g3-alt gen_perm n=1000 mode=alternating
tc g3-bitonic gen_perm n=1000 mode=bitonic
tc g3-rec gen_perm n=1000 mode=recursive
tc g3-blocks-20 gen_perm n=1000 mode=blocks block=20
tc g3-blocks-63 gen_perm n=1000 mode=blocks block=63

group group4 28
limits maxn=300000
tc g4-rand-20000 gen_rand n=20000
tc g4-rand-50000 gen_rand n=50000
tc g4-rand-299999 gen_rand n=299999
tc g4-rand1 gen_rand n=300000
tc g4-rand2 gen_rand n=300000
tc g4-rand3 gen_rand n=300000
tc g4-rand4 gen_rand n=300000
tc g4-rand5 gen_rand n=300000

group group5 35
limits maxn=300000
include_group group3
include_group group4
tc g5-inc gen_perm n=300000 mode=inc
tc g5-dec gen_perm n=300000 mode=dec
tc g5-alt gen_perm n=300000 mode=alternating
tc g5-bitonic gen_perm n=300000 mode=bitonic
tc g5-rec gen_perm n=300000 mode=recursive
tc g5-blocks-17 gen_perm n=300000 mode=blocks block=17
tc g5-blocks-316 gen_perm n=300000 mode=blocks block=316
tc g5-blocks-999 gen_perm n=300000 mode=blocks block=999
