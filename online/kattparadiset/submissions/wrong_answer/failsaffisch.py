#!/usr/bin/python3

# This should fail
"""
13 3
gul 4
brun 4
rosa 5
"""

import heapq

n,k = map(int,input().split())

largest = 0

priority_queue = []
for _ in range(k):
    color, amount = input().split()

    largest = max(largest, int(amount))

    # heapq is a max-heap, so we flip the sign of amount
    heapq.heappush(priority_queue, (-int(amount), color))

if largest > n//2:
    print("Nej")
    exit()

out = [""]*n
for i in range(n):
    amount, color = heapq.heappop(priority_queue)

    if color != out[i-1]:
        out[i] = color
        heapq.heappush(priority_queue, (amount+1, color))
    else:
        amount2, color2 = heapq.heappop(priority_queue)
        out[i] = color2

        heapq.heappush(priority_queue, (amount, color))
        heapq.heappush(priority_queue, (amount2+1, color2))

print("Ja")
print(*out)

