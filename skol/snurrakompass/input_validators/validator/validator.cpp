#include "validator.h"
using namespace std;

void run() {
	int a = Int(1, 360);
	Endl();
	int b = Int(1, 360);
	Endl();

	if (Arg("ab", 0)) assert(a == b);
	if (Arg("prime", 0)) assert(a == 13);
}

