n, s = [int(i) for i in input().split()]

x = [0]*n
t = [0]*n
for i in range(n):
    x[i], t[i] = [int(i) for i in input().split()]

real = [1e18]*n
nether = [1e18]*n

for i in range(n):
    real[i] = 8 * x[i]
    nether[i] = real[i] + t[i]
    if i > 0:
        nether[i] = min(nether[i], nether[i-1] + x[i]-x[i-1])

ans = s
for i in range(n):
    ans = min(ans, real[i] + s - 8 * x[i])
    ans = min(ans, nether[i] + t[i] + s - 8 *x[i])

print(ans)