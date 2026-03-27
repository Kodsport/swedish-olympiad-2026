#!/usr/bin/env python3

import random
import sys


def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=", 1)[1]
    if default is None:
        print("missing parameter", name)
        sys.exit(1)
    return default


random.seed(int(cmdlinearg("seed", sys.argv[-1])))
n = int(cmdlinearg("n"))

perm = list(range(1, n + 1))
random.shuffle(perm)

print(n)
print(*perm)
