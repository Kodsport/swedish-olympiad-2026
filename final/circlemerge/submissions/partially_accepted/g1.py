n = int(input())
a = [*map(int,input().split())]

s = sum(a)

print(n-s//a[0])
