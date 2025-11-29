n=2*10**5
t=10**6
from random import randint as rand
from itertools import combinations

items = [(1,3)]
target_size = 5

x = 0
div = 5
while x < t:
    mul = rand(1, min(10, (t-x)//5))
    items.append((rand(3,5),mul*5))
    x += mul * 5
print(len(items),t)
for p,w in items:
    print(p,w)
