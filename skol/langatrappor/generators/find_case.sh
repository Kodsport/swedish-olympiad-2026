while true; do
    python3 gen_parabola.py r=700000000 c=10 > in.txt
    ./bug < in.txt > bug.txt
    ./corr < in.txt > corr.txt
    diff bug.txt corr.txt || exit 0
    printf .
done