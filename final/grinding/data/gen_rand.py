#!/usr/bin/python3

import sys
import random
import math
from random import randint

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
t = int(cmdlinearg('t'))
b = int(cmdlinearg('b'))
monster_distribution = cmdlinearg('monster_distribution')
stat_mode = cmdlinearg('stat_mode')

print(n, b)

distribution = [1] * n
num_remaining = t-n
if monster_distribution == "random":
    for _ in range(num_remaining):
        distribution[randint(0, n-1)] += 1
elif monster_distribution == "even":
    for i in range(num_remaining):
        distribution[i % n] += 1
elif monster_distribution == "frontloaded":
    for i in range(num_remaining):
        distribution[i * n // t] += 1
elif monster_distribution == "backloaded":
    for i in range(num_remaining):
        distribution[(n - 1) - (i * n // t)] += 1
elif monster_distribution == "spiky":
    spikes = int(cmdlinearg('spikes'))
    amount = num_remaining // spikes
    for s in range(spikes):
        spike_pos = randint(0, n - 1)
        for _ in range(amount):
            distribution[spike_pos] += 1
else:
    assert False

assert sum(distribution) <= t

placed_strong_monster = False
for i in range(n):
    num_monsters = distribution[i]
    print(num_monsters)
    for j in range(num_monsters):
        if stat_mode == "random":
            strength = randint(1, max(0, b - 1))
            xp = randint(1, max(1, b - 1))
        elif stat_mode == "strong":
            strength = randint(1, b - 1)
            xp = randint(1, 10)
        elif stat_mode == "weak":
            strength = randint(1, 10)
            xp = randint(1, 10)
        elif stat_mode == "ratio":
            xp = randint(1, 10)
            strength = xp * randint(1, 10) + randint(0, 5)
        elif stat_mode == "single_great":
            xp = randint(1, 10)
            strength = xp * randint(1, 10) + randint(0, 5)
            if i * 2 > n and not placed_strong_monster:
                strength = 1
                xp = b // 10
                placed_strong_monster = True
        else:
            assert False
        if i == 0 and j == 0:
            strength = 1
        print(strength, xp)
