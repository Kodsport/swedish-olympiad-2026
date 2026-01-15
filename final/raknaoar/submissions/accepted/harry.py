#!/usr/bin/python3
n,q = map(int,input().split())

ans = (0,n)
queries = [0]*q
cnt = [0]*(n+1)

islands = []
for i in range(n):
    a = [*map(int,input().split())]
    for x in a[1:]:
        islands.append((x,i+1))

islands.sort()

questions = [(int(input()),i) for i in range(q)]

questions.sort()

while questions and islands:
    if questions[-1][0] <= islands[-1][0]:
        x,i = islands.pop()
        cnt[i] += 1
        ans = max(ans,(cnt[i],i))
    else:
        x,i = questions.pop()
        queries[i] = ans[1]

while questions:
    x,i = questions.pop()
    queries[i] = ans[1]

for x in queries:
    print(x)
