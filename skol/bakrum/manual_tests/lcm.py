import math

def lcm(a, b):
    return abs(a*b) // math.gcd(a, b)

# vals = []
# current = 1
# for n in range(1, 101):
#     current = lcm(current, n)
#     print(f"{n}: {current} {math.log10(current)}")

alive_d = list(range(1, 1001))

all_nums = []
while len(alive_d):
    #print(len(alive_d))
    current = 1
    for v in alive_d:
        nxt = lcm(current, v)
        if nxt * 4 > 10**5:
            break
        current = nxt
        #print(f"{v}: {current} {math.log10(current)}")
    #print(f"{current}")
    if current*3+1 not in all_nums:
        all_nums.append(3*current+1)
    alive_d = [d for d in alive_d if nxt % d != 0]
print(len(all_nums))
print(all_nums)
i=1
seen = set()

while len(all_nums):
    v = []
    for _ in range(5):
        if len(all_nums) == 0:
            break
        v.append(all_nums.pop())

    
    with open(f"kill_nils/kill_nils_medium/nils_killer_medium{i}.in", "w") as f:
        f.write("""3 3
###
#.#
###\n""")
        for x in v:
            f.write(f"1 1 {x} 1\n")
    i+=1
