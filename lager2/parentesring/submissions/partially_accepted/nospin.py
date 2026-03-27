# @EXPECTED_GRADES@ WA WA WA AC WA WA WA WA

n,a,b = map(int,input().split())
s = input()

l = s.count("(")
h = s.count(")")

print(abs(l-h)*a)
