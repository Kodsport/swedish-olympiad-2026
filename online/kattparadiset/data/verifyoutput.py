input()
a = input().split()

for i in range(len(a)):
    if a[i] == a[(i+1)%len(a)]:
        print(i,a[i],a[(i+1)%len(a)])
        exit()