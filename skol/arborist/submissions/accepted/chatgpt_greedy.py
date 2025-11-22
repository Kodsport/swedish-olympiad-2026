#!/usr/bin/python3
def main():
    N, K = map(int, input().split())
    xs = list(map(int, input().split()))
    ws = list(map(int, input().split()))

    # Samla ihop träd som (x, w)
    trees = [(xs[i], ws[i], i) for i in range(N)]
    # Sortera efter x fallande (längst bort först)
    trees.sort(reverse=True)

    planted = [False] * N
    total_time = 0

    remaining = N
    while remaining > 0:
        weight = 0
        count = 0
        max_x = 0

        # Först: hitta första otagna (den längst bort)
        for pos, w, idx in trees:
            if not planted[idx] and weight + w <= K:
                weight += w
                planted[idx] = True
                remaining -= 1
                count += 1
                if pos > max_x:
                    max_x = pos
                break  # bara ta EN som "start"

        # Sedan: fyll upp med så många små som får plats
        for pos, w, idx in trees:
            if remaining == 0:
                break
            if not planted[idx] and weight + w <= K:
                weight += w
                planted[idx] = True
                remaining -= 1
                count += 1
                if pos > max_x:
                    max_x = pos

        total_time += 2 * max_x + 2 * count

    print(total_time)


if __name__ == "__main__":
    main()
