
n = 2871
#base = 2248776129600
#base = 1927522396800
base = 91
a = [base<<1] + [base]*1433 + [base<<1] + [base]*(1434)

a = a[10:] + a[:10]
print(len(a))
print(*a)