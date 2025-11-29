n=2*10**5
t=10**6
from random import randint as rand
from itertools import combinations

div = 125
def valid(cand):
    if sum(x[0] for x in cand) > 4:
        return False
    vals = [x[1] for x in cand]
    return all(sum(sub) % div != 0 for r in range(1, len(vals)+1) for sub in combinations(vals, r))

items = []
target_size = 2

while target_size > 0:
    for _ in range(1000):
        cand = [(rand(2,4), rand(1,50)) for _ in range(target_size)]
        if valid(cand):
            items+=cand
            target_size = 0
            break
    else:
        target_size -= 1
x = 0
while x < t:
    mul = rand(1, min(10, (t-x)//div))
    items.append((5,mul*div))
    x += mul * div
print(len(items),t)
for p,w in items:
    print(p,w)
