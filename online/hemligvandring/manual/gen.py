#!/usr/bin/python3
import random
import string

names = set(["ERDOS"])
while len(names) < 100000:
    names.add("".join(random.choices(string.ascii_uppercase*random.randint(10,10), k=random.randint(10,10))))

names = list(names)
print(len(names),2)
print(len(names)-1,*names[:-1])
print(2,*names[-2:])