n, v = list(map(int, input().split()))
arr = list(map(int, input().split()))

brr = []
for i in range(n):
    if arr[i] > v + len(brr):
        brr.append(arr[i])

for i in range(len(brr)-2, -1, -1):
    brr[i] = max(brr[i], brr[i+1])

print(len(brr))
print(*brr)