#!/usr/bin/python3

r,c=map(int, input().split())
row = list(input())

coords = [i for i in range(c) if row[i]=='*']
ans_pos = (coords[0]+coords[1])//2
row[ans_pos] = 'X'
print(''.join(row))
