#!/bin/bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution joshua_chatgpt.cpp

compile gen_rand.py
compile gen2.py

samplegroup
sample sample01
sample sample02

group group1 40
limits maxn=10
include_group sample
tc g1-1 gen_rand n=2 m=2
tc g1-2 gen_rand n=5 m=3
tc g1-3 gen_rand n=10 m=5
tc g1-4 gen_rand n=10 m=10
tc g1-5 gen_rand n=10 m=8 locs=2 hics=5
tc g1-6 gen_rand n=10 m=3 locs=2 hics=10
tc g1-7 gen_rand n=7 m=7 mode=star
tc g1-8 gen_rand n=8 m=4 mode=line
tc g1-9 gen_rand n=9 m=6 mode=tree
tc g1-10 gen_rand n=10 m=2 mode=bigclique

group group2 60
include_group group1
tc g2-1 gen_rand n=50 m=40
tc g2-5 gen_rand n=10000 m=9000
tc g2-6 gen_rand n=10000 m=30000
tc g2-7 gen_rand n=10000 m=20000 locs=2 hics=10
tc g2-8 gen_rand n=60000 m=100000 mode=star
tc g2-9 gen_rand n=60000 m=100000 mode=line
tc g2-10 gen_rand n=60000 m=100000 mode=tree
tc g2-11 gen_rand n=60000 m=100000 mode=bigclique
tc g2-12 gen_rand n=20000 m=40000 locs=2 hics=5
tc g2-13 gen_rand n=50000 m=100000 mode=star
tc g2-14 gen_rand n=75000 m=100000 mode=line
tc g2-15 gen_rand n=100000 m=100000 mode=tree
tc g2-16 gen_rand n=100000 m=100000 mode=bigclique
tc_manual ../manual/bigcase1.in
tc_manual ../manual/bigcase2.in
tc g2-17 gen2 n=10000 publication_size=10
tc g2-18 gen2 n=10000 publication_size=40
tc g2-19 gen2 n=10000 publication_size=100
tc g2-20 gen2 n=10000 publication_size=1000
tc g2-21 gen2 n=10000 publication_size=5000
