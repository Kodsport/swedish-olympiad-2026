# Throw random heuristics at it

import sys
import math

def greedy_take(castles, T, score_fn):
    arr = sorted(castles, key=lambda lg: score_fn(lg[0], lg[1]), reverse=True)
    troops = T
    val = 0
    for l,g in arr:
        if g <= troops:
            troops -= g
            val += l
    return val

def main():
    data = sys.stdin.read().strip().split()
    it = iter(data)
    N = int(next(it))
    T = int(next(it))
    castles = []
    for _ in range(N):
        l = int(next(it)); g = int(next(it))
        if g <= T:
            castles.append((l,g))

    if not castles:
        print(0)
        return

    candidates = []

    heuristics = [
        ("ratio_l_over_g",                          lambda l,g: l / g),                    # klassisk densitet
        ("ratio_l_over_sqrt_g",                     lambda l,g: l / math.sqrt(g)),         # mildare straff för stort g
        ("ratio_l_over_g_pow1_2",                   lambda l,g: l / (g**1.2)),             # aggressivare straff
        ("ratio_l_over_log_g",                      lambda l,g: l / (1 + math.log(1+g))),  # log-variant
        ("value_minus_cost",                        lambda l,g: l - g),                    # absolut nytta minus kostnad
        ("value_minus_halfcost",                    lambda l,g: l - 0.5*g),                # svag kostnadspåverkan
        ("value_minus_twocost",                     lambda l,g: l - 2*g),                  # hård kostnadspåverkan
        ("value_squared_over_cost",                 lambda l,g: (l*l) / g if g>0 else float('inf')), # stor-l prioritet
        ("high_value_first",                        lambda l,g: l),                        # max l först
        ("low_cost_first",                          lambda l,g: -g),                       # min g först (små kostnader prioriteras)
        ("ratio_with_cost_penalty",                 lambda l,g: (l / g) - 0.01*g),         # densitet med linjär kostnadspenalty
        ("value_plus_inverse_cost",                 lambda l,g: l + (1.0 / (g+1))),        # föredra små g som tie-break
    ]

    for name, fn in heuristics:
        val = greedy_take(castles, T, score_fn=fn)
        candidates.append((name, val))


    # Skriv ut bästa av alla heuristiker
    best_name, best_val = max(candidates, key=lambda nv: nv[1])
    print(best_val)

if __name__ == "__main__":
    main()
