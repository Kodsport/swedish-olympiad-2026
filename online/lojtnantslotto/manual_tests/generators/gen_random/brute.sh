while true; do
    pypy3 ../data/gen_rand.py mode=random n=200000 t=1000000 maxl=2 > in.txt
    #pypy3 ../data/gen_harder.py mode=corr n=2000 t=1000000 l=5 mul=1000 add=1000 > in.txt
    ./a.out < in.txt > out.txt
    ./joshua < in.txt > corr.txt
    diff out.txt corr.txt || exit 0
    printf .
done
