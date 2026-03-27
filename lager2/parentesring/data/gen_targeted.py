#!/usr/bin/python3

import sys
import random

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
a = int(cmdlinearg('a'))
b = int(cmdlinearg('b'))
mode = cmdlinearg('mode')

if mode == 'all_left':
    s = ['('] * n

elif mode == 'all_right':
    s = [')'] * n

elif mode == 'nested':
    half = n // 2
    s = ['('] * half + [')'] * half
    if n % 2 == 1:
        s.append(random.choice('()'))

elif mode == 'half_half':
    # )))...((( - worst case rotation for equal parens
    half = n // 2
    s = [')'] * half + ['('] * half
    if n % 2 == 1:
        s.append(random.choice('()'))

elif mode == 'alternating':
    s = list('()' * (n // 2))
    if n % 2 == 1:
        s.append(random.choice('()'))

elif mode == 'rev_alternating':
    s = list(')(' * (n // 2))
    if n % 2 == 1:
        s.append(random.choice('()'))

elif mode == 'skewed':
    left_count = int(cmdlinearg('left'))
    s = ['('] * left_count + [')'] * (n - left_count)
    random.shuffle(s)

else:
    assert False, f"Unknown mode: {mode}"

spin = int(cmdlinearg('spin', '0'))
if spin:
    s = s[spin:] + s[:spin]

swaps = int(cmdlinearg('swaps', '0'))
for _ in range(int(swaps)):
    i, j = random.randint(0, len(s)-1), random.randint(0, len(s)-1)
    s[i], s[j] = s[j], s[i]

print(n, a, b)
print(''.join(s))
