#!/usr/bin/env bash

PPATH=$(realpath ..)

. ../../../testdata_tools/gen.sh

#ulimit -s unlimited

use_solution joshua_9.cpp opt

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
tc g1-concentrated1 gen_rand n=$N mode=concentrated amounts=1
tc g1-concentrated2 gen_rand n=$N mode=concentrated amounts=1
tc g1-concentrated3 gen_rand n=$N mode=concentrated amounts=1
tc g1-concentrated4 gen_rand n=$N mode=concentrated amounts=1
tc g1-concentrated5 gen_rand n=$N mode=concentrated amounts=1
tc g1-concentrated6 gen_rand n=$N mode=concentrated amounts=1
tc g1-concentrated7 gen_rand n=$N mode=concentrated amounts=2
tc g1-concentrated8 gen_rand n=$N mode=concentrated amounts=2
tc g1-concentrated9 gen_rand n=$N mode=concentrated amounts=3
tc g1-concentrated10 gen_rand n=$N mode=concentrated amounts=4
tc g1-concentrated11 gen_rand n=$N mode=concentrated amounts=8
tc g1-concentrated12 gen_rand n=$N mode=concentrated amounts=16
tc g1-concentrated13 gen_rand n=$N mode=concentrated amounts=32
tc g1-concentrated14 gen_rand n=$N mode=concentrated amounts=64
tc g1-concentrated15 gen_rand n=$N mode=concentrated amounts=128
tc g1-concentrated16 gen_rand n=$N mode=concentrated amounts=256
tc g1-spread1 gen_rand n=$N mode=spread
tc g1-spread2 gen_rand n=$N mode=spread
tc g1-concentrate_around1 gen_rand n=$N mode=concentrated_2 target=0 other=0 specific_other=-1
tc g1-concentrate_around2 gen_rand n=$N mode=concentrated_2 target=0 other=1 specific_other=255
tc g1-concentrate_around3 gen_rand n=$N mode=concentrated_2 target=0 other=1 specific_other=254
tc g1-concentrate_around4 gen_rand n=$N mode=concentrated_2 target=0 other=1 specific_other=253
tc g1-concentrate_around5 gen_rand n=$N mode=concentrated_2 target=0 other=250 specific_other=255
tc g1-concentrate_around6 gen_rand n=$N mode=concentrated_2 target=0 other=250 specific_other=254
tc g1-concentrate_around7 gen_rand n=$N mode=concentrated_2 target=0 other=250 specific_other=253
tc g1-concentrate_around8 gen_rand n=$N mode=concentrated_2 target=0 other=250 specific_other=-1
tc g1-concentrate_around9 gen_rand n=$N mode=concentrated_2 target=0 other=250 specific_other=-1
tc g1-concentrate_around10 gen_rand n=$N mode=concentrated_2 target=0 other=250 specific_other=-1
tc g1-dup1 gen_rand n=$N mode=single_dup dup=0 
tc g1-dup2 gen_rand n=$N mode=single_dup dup=1 
tc g1-dup3 gen_rand n=$N mode=single_dup dup=63 
tc g1-dup4 gen_rand n=$N mode=single_dup dup=64
tc g1-dup5 gen_rand n=$N mode=single_dup dup=127
tc g1-dup6 gen_rand n=$N mode=single_dup dup=128
tc g1-dup7 gen_rand n=$N mode=single_dup dup=255
tc g1-dup10 gen_rand n=$N mode=single_dup dup=48
tc g1-dup11 gen_rand n=$N mode=single_dup dup=107
tc g1-dup12 gen_rand n=$N mode=single_dup dup=135
tc g1-dup13 gen_rand n=$N mode=single_dup dup=143
tc g1-dup14 gen_rand n=$N mode=single_dup dup=3
tc g1-dup15 gen_rand n=$N mode=single_dup dup=7
tc g1-dup16 gen_rand n=$N mode=single_dup dup=149
