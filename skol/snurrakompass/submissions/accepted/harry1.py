#!/usr/bin/python3

import math
a,b = map(int,input().split())
print(360//math.gcd(a,math.gcd(b,360)))
