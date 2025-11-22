#!/usr/bin/python3

import math
a=int(input())
b=int(input())
print(360//math.gcd(a,math.gcd(b,360)))
