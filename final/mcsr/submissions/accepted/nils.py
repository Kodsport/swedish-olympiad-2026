#!/usr/bin/python3

n,s = map(int,input().split())

X = [0]
T = [10**20]
for i in range(n):
    x,t = map(int,input().split())
    X.append(x)
    T.append(t)

over = s-8*X[-1]
nether = T[-1]+over

for i in range(n)[::-1]:
    over, nether = min(8*X[i+1]-8*X[i]+over, T[i]+ X[i+1]-X[i]+nether), min(T[i]+8*X[i+1]-8*X[i]+over, X[i+1]-X[i]+nether)

print(over)

