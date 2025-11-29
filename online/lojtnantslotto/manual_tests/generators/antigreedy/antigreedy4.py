n=2*10**5
t=10**6
from random import randint as rand

items = [(1,1)]
x = 0
div = 5
while x < t:
    mul = rand(1, min(10, (t-x)//div))
    items.append((2,mul*div))
    x += mul * div
print(len(items),t)
for p,w in items:
    print(p,w)
