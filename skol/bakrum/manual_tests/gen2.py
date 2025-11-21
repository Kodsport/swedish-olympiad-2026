grid="""###
#.#
###"""
R=3
C=3
print(R,C)
print(grid)

D=232792560 # lcm(1,...22)
queries_str = f"""1 1 1 1
1 1 1 {1 + 3*D}
1 1 {1 + 3*D} 1
1 1 {1 + 6*D} {1 + 6*D}
4 7 {4 + 3*D} 7"""
print(queries_str)