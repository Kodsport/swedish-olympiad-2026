#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../testdata_tools/gen.sh

#ulimit -s unlimited

use_solution joshua_9.cpp

compile gen_rand.py

# samplegroup
# sample 1
# sample 2
# sample 3

N=512
group g1 100
tc g1-rand1 gen_rand n=$N mode=random
tc g1-rand2 gen_rand n=$N mode=random
tc g1-rand3 gen_rand n=$N mode=random
tc g1-rand4 gen_rand n=$N mode=random
tc g1-big1 gen_rand  n=$N mode=big num_rand=0
tc g1-big2 gen_rand  n=$N mode=big num_rand=5
tc g1-big3 gen_rand  n=$N mode=big num_rand=50
tc g1-big4 gen_rand  n=$N mode=big num_rand=250
tc g1-threshold1 gen_rand  n=$N mode=num_big num_big=0
tc g1-threshold2 gen_rand  n=$N mode=num_big num_big=0
tc g1-threshold3 gen_rand  n=$N mode=num_big num_big=63
tc g1-threshold4 gen_rand  n=$N mode=num_big num_big=64
tc g1-threshold5 gen_rand  n=$N mode=num_big num_big=65
tc g1-threshold6 gen_rand  n=$N mode=num_big num_big=127
tc g1-threshold7 gen_rand  n=$N mode=num_big num_big=128
tc g1-threshold8 gen_rand  n=$N mode=num_big num_big=129
tc g1-threshold9 gen_rand  n=$N mode=num_big num_big=254
tc g1-threshold10 gen_rand  n=$N mode=num_big num_big=255
tc g1-threshold11 gen_rand  n=$N mode=num_big num_big=256
tc g1-threshold12 gen_rand  n=$N mode=num_big num_big=510
tc g1-threshold13 gen_rand  n=$N mode=num_big num_big=511
tc g1-threshold14 gen_rand  n=$N mode=num_big num_big=512
