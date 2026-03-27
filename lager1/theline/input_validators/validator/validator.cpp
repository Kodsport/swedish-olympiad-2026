#include "validator.h"
using namespace std;


const int MAX_N = 1e9;
const int MAX_K = 1e4;

void run() {
    int n = Int(1, Arg("maxn", MAX_N));
    Space();
    int k = Int(1, Arg("maxk", MAX_K));
    Endl();

    for (int i = 0; i < k; i++) {
        Int(1, n);
        Space();
        Int(1, n);
        Endl();
    }
}
