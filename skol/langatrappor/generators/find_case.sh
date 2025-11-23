while true; do
    python3 gen_parabola.py r=100000000 c=10 > in.txt
    ./bug.out < in.txt > bug.txt
    ./corr.out < in.txt > corr.txt
    diff bug.txt corr.txt || exit 0
    printf .
done
