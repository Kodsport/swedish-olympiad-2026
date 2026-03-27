#!/usr/bin/env python3
import sys

INF = 10**9
OUT = 0
DOM = 1
SEL = 2
SHIFT = 20
POW3 = (1, 3, 9, 27)


def build_digits(length):
    digits = []
    for state in range(POW3[length]):
        cur = []
        value = state
        for _ in range(length):
            cur.append(value % 3)
            value //= 3
        digits.append(tuple(cur))
    return tuple(digits)


DIGITS = tuple(build_digits(length) for length in range(4))


def build_base(length, local_pos):
    costs = [INF] * POW3[length]
    active = []
    for mask in range(1 << length):
        any_selected = mask != 0
        state = 0
        mul = 1
        for i in range(length):
            if (mask >> i) & 1:
                digit = SEL
            elif any_selected:
                digit = DOM
            else:
                digit = OUT
            state += digit * mul
            mul *= 3
        costs[state] = 1 if (mask >> local_pos) & 1 else 0
        active.append(state)
    return tuple(costs), tuple(active)


def build_projection(length, local_pos):
    projection = [-1] * POW3[length]
    for state, digits in enumerate(DIGITS[length]):
        if digits[local_pos] == OUT:
            continue
        new_state = 0
        mul = 1
        for i, digit in enumerate(digits):
            if i == local_pos:
                continue
            new_state += digit * mul
            mul *= 3
        projection[state] = new_state
    return tuple(projection)


def build_merge(parent_len, child_len, positions):
    table = []
    for parent_state, parent_digits in enumerate(DIGITS[parent_len]):
        row = [-1] * POW3[child_len]
        for child_state, child_digits in enumerate(DIGITS[child_len]):
            merged = list(parent_digits)
            for idx, pos in enumerate(positions):
                left = parent_digits[pos]
                right = child_digits[idx]
                if (left == SEL) != (right == SEL):
                    break
                if left == SEL:
                    merged[pos] = SEL
                elif left == DOM or right == DOM:
                    merged[pos] = DOM
                else:
                    merged[pos] = OUT
            else:
                new_state = 0
                mul = 1
                for digit in merged:
                    new_state += digit * mul
                    mul *= 3
                row[child_state] = new_state
            continue
        table.append(tuple(row))
    return tuple(table)


BASE_COSTS = [None] * 4
BASE_ACTIVE = [None] * 4
PROJECTION = [None] * 4
for length in range(1, 4):
    base_costs = []
    base_active = []
    projections = []
    for local_pos in range(length):
        costs, active = build_base(length, local_pos)
        base_costs.append(costs)
        base_active.append(active)
        projections.append(build_projection(length, local_pos))
    BASE_COSTS[length] = tuple(base_costs)
    BASE_ACTIVE[length] = tuple(base_active)
    PROJECTION[length] = tuple(projections)

MERGE = {}
for parent_len in range(1, 4):
    for child_len in range(1, min(2, parent_len) + 1):
        for positions in ((0,), (1,), (2,), (0, 1), (0, 2), (1, 2)):
            if len(positions) != child_len:
                continue
            if any(pos >= parent_len for pos in positions):
                continue
            MERGE[(parent_len, child_len, positions)] = build_merge(
                parent_len, child_len, positions
            )


def pair_key(a, b):
    if a > b:
        a, b = b, a
    return (a << SHIFT) | b


def solve(p):
    n = len(p)
    sep_size = [0] * n
    sep0 = [0] * n
    sep1 = [0] * n
    order = []
    stack = []

    for i, value in enumerate(p):
        while stack and p[stack[-1]] < value:
            node = stack.pop()
            if stack:
                left = stack[-1]
                right = i
                if left < right:
                    sep0[node] = left
                    sep1[node] = right
                else:
                    sep0[node] = right
                    sep1[node] = left
                sep_size[node] = 2
            else:
                sep0[node] = i
                sep_size[node] = 1
            order.append(node)
        stack.append(i)

    root = -1
    while stack:
        node = stack.pop()
        if stack:
            sep0[node] = stack[-1]
            sep_size[node] = 1
        else:
            root = node
        order.append(node)

    local_pos = [0] * n
    first_child = [-1] * n
    next_sibling = [-1] * n
    single_owner = [-1] * n
    pair_owner = {}

    for node in reversed(order):
        size = sep_size[node]
        if size == 0:
            local_pos[node] = 0
            single_owner[node] = node
            continue

        if size == 1:
            other = sep0[node]
            parent = single_owner[other]
            if node < other:
                left = node
                right = other
                local_pos[node] = 0
            else:
                left = other
                right = node
                local_pos[node] = 1
            single_owner[left] = node
            single_owner[right] = node
            pair_owner[pair_key(left, right)] = node
        else:
            left = sep0[node]
            right = sep1[node]
            parent = pair_owner[pair_key(left, right)]
            if node < left:
                a = node
                b = left
                c = right
                local_pos[node] = 0
            elif node < right:
                a = left
                b = node
                c = right
                local_pos[node] = 1
            else:
                a = left
                b = right
                c = node
                local_pos[node] = 2
            single_owner[a] = node
            single_owner[b] = node
            single_owner[c] = node
            pair_owner[pair_key(a, b)] = node
            pair_owner[pair_key(a, c)] = node
            pair_owner[pair_key(b, c)] = node

        next_sibling[node] = first_child[parent]
        first_child[parent] = node

    dp_cost = [None] * n
    dp_active = [None] * n

    for node in order:
        size = sep_size[node]
        bag_len = size + 1
        pos = local_pos[node]
        cur = list(BASE_COSTS[bag_len][pos])
        active = list(BASE_ACTIVE[bag_len][pos])

        if size == 0:
            bag0 = node
            bag1 = bag2 = -1
        elif size == 1:
            other = sep0[node]
            if pos == 0:
                bag0 = node
                bag1 = other
            else:
                bag0 = other
                bag1 = node
            bag2 = -1
        else:
            left = sep0[node]
            right = sep1[node]
            if pos == 0:
                bag0 = node
                bag1 = left
                bag2 = right
            elif pos == 1:
                bag0 = left
                bag1 = node
                bag2 = right
            else:
                bag0 = left
                bag1 = right
                bag2 = node

        child = first_child[node]
        while child != -1:
            child_size = sep_size[child]
            child_cost = dp_cost[child]
            child_active = dp_active[child]

            if bag_len == 1:
                positions = (0,)
            elif bag_len == 2:
                if child_size == 1:
                    positions = (0,) if sep0[child] == bag0 else (1,)
                else:
                    positions = (0, 1)
            else:
                if child_size == 1:
                    child_vertex = sep0[child]
                    if child_vertex == bag0:
                        positions = (0,)
                    elif child_vertex == bag1:
                        positions = (1,)
                    else:
                        positions = (2,)
                else:
                    left = sep0[child]
                    right = sep1[child]
                    if left == bag0:
                        positions = (0, 1) if right == bag1 else (0, 2)
                    else:
                        positions = (1, 2)

            table = MERGE[(bag_len, child_size, positions)]
            new = [INF] * POW3[bag_len]
            new_active = []
            for parent_state in active:
                base_cost = cur[parent_state]
                row = table[parent_state]
                for child_state in child_active:
                    merged_state = row[child_state]
                    if merged_state == -1:
                        continue
                    value = base_cost + child_cost[child_state]
                    if value < new[merged_state]:
                        if new[merged_state] == INF:
                            new_active.append(merged_state)
                        new[merged_state] = value
            cur = new
            active = new_active
            child = next_sibling[child]

        result = [INF] * POW3[size]
        result_active = []
        projection = PROJECTION[bag_len][pos]
        for state in active:
            projected = projection[state]
            if projected == -1:
                continue
            value = cur[state]
            if value < result[projected]:
                if result[projected] == INF:
                    result_active.append(projected)
                result[projected] = value

        dp_cost[node] = result
        dp_active[node] = result_active

    return dp_cost[root][0]


def main() -> None:
    input = sys.stdin.readline
    _ = int(input())
    p = list(map(int, input().split()))
    print(solve(p))


if __name__ == "__main__":
    main()
