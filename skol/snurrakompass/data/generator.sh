#!/usr/bin/env bash

PPATH=$(realpath ..)
. ../../../testdata_tools/gen.sh

use_solution raunak.cpp

compile gen_rand.py

samplegroup
sample 1
sample 2

group group1 20
limits prime=1
for i in {1..20}; do
    tc g1-$i gen_rand mode=prime
done
tc_manual ../skolkval_tests/skol_1.in

group group2 40
limits ab=1
tc 2
tc g2-1 gen_rand mode=ab
tc g2-2 gen_rand mode=ab
tc g2-3 gen_rand mode=ab
tc g2-4 gen_rand mode=ab
tc g2-5 gen_rand mode=ab
tc g2-6 gen_rand mode=ab
tc g2-7 gen_rand mode=ab
tc g2-8 gen_rand mode=hardcode a=359 b=359
tc g2-10 gen_rand mode=hardcode a=1 b=1
for i in {1..10}; do
    tc g2-small$i gen_rand mode=hardcode a=$i b=$i
done
tc_manual ../skolkval_tests/skol_2.in
tc_manual ../skolkval_tests/skol_3.in

group group3 40
include_group sample group1 group2
for i in {1..10}; do
    tc g3-$i gen_rand mode=random
done
for i in {11..15}; do
    tc g3-multiple-$i gen_rand mode=multiple
done
tc g3-hc-ans3 gen_rand mode=hardcode a=120 b=240
tc g3-hc-ans4 gen_rand mode=hardcode a=90 b=180
tc g3-hc-ans5 gen_rand mode=hardcode a=72 b=144
tc g3-hc-ans6 gen_rand mode=hardcode a=60 b=120
tc g3-hc-ans9 gen_rand mode=hardcode a=40 b=80
tc g3-hc-ans10 gen_rand mode=hardcode a=36 b=72
tc g3-hc-ans12 gen_rand mode=hardcode a=30 b=60
tc g3-hc-ans18 gen_rand mode=hardcode a=20 b=40
tc g3-hc-ans24 gen_rand mode=hardcode a=15 b=30
tc g3-hc-ans30 gen_rand mode=hardcode a=12 b=24

tc g3-hc-1-1 gen_rand mode=hardcode a=1 b=1
tc g3-hc-1-360 gen_rand mode=hardcode a=1 b=360
tc g3-hc-360-7 gen_rand mode=hardcode a=360 b=7
tc g3-hc-360-8 gen_rand mode=hardcode a=360 b=8
tc g3-hc-360-180 gen_rand mode=hardcode a=360 b=180
tc g3-hc-180-180 gen_rand mode=hardcode a=180 b=180
tc g3-hc-180-360 gen_rand mode=hardcode a=180 b=360

tc g3-hc-adv1 gen_rand mode=hardcode a=81 b=241
tc g3-hc-adv2 gen_rand mode=hardcode a=8 b=154
tc g3-hc-adv3 gen_rand mode=hardcode a=147 b=221

tc g3-hc-cop1 gen_rand mode=hardcode a=7 b=11
tc g3-hc-cop2 gen_rand mode=hardcode a=49 b=7

tc_manual ../skolkval_tests/skol_4.in
tc_manual ../skolkval_tests/skol_5.in
