#include "validator.h"

using namespace std;

void run() {
	int maxval = Arg("maxval",30);

	int a = Int(0, maxval);
	Endl();
	int b = Int(0, maxval);
	Endl();

	if (Arg("a_neq_b", 0)) assert(a != b);
	if (Arg("jack_not_win", 0)) {
		assert(min(a,b) <= 21);
		assert(a != b);
	}
}
