#!/usr/bin/python3

import sys
import random

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=", 1)[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))

n = int(cmdlinearg('n'))
T = int(cmdlinearg('t'))

print(n, T)
base = n // 5
rem = n % 5
counts = [base + (1 if i < rem else 0) for i in range(5)]

for s_idx in range(5):
    s = s_idx + 1
    for _ in range(counts[s_idx]):
        print(s, 1)
