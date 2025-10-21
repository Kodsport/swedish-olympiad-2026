#!/usr/bin/python3

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

# Take the largest color and fill out every other position
amount, color = heapq.heappop(priority_queue)
for i in range(0,n,2):
    if amount != 0:
        out[i] = color
        amount += 1


for i in range(n):
    if out[i] != "":
        continue

    amount,color = heapq.heappop(priority_queue)

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

