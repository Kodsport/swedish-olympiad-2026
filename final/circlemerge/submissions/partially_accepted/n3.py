# O(n^3)

n = int(input())
a = [*map(int,input().split())]

ans = n-1

for start in range(n):
    cur = 0
    for end in range(start,n):
        cur += a[end]
        
        intervals = 1

        temp = 0

        j = (end+1)%n
        
        bad = 0
        while j != start:
            temp += a[j]
            j = (j+1)%n

            if temp > cur:
                bad = 1
                break
            if temp == cur:
                temp -= cur
                intervals += 1

        if not bad and temp == 0:
            ans = min(ans,n-intervals)

print(ans)