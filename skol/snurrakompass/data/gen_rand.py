#!/usr/bin/python3

import sys
import random
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

mode = cmdlinearg('mode')

if mode == 'prime':
    a = 13
    b = randint(1, 360)
    print(a)
    print(b)
elif mode == 'ab':
    a = randint(1, 360) 
    print(a)
    print(a)
elif mode == 'random':
    a = randint(1, 360)
    b = randint(1, 360)
    print(a)
    print(b)
elif mode == 'hardcode':
    print(cmdlinearg('a'))
    print(cmdlinearg('b'))
elif mode == 'multiple':
    a = randint(1, 360)
    b = a * randint(1, 100)
    print(a)
    print(((b-1) % 360)+1)
else:
    assert 0

