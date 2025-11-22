#include "validator.h"

using namespace std;

void run() {
	int r = Int(1, Arg("maxr", int(1e9)));
	Space();
	int c = Int(1, 10);
	Endl();

	int c_is = Arg("c_is", -1);
	if (c_is != -1) assert(c == c_is);

	SpacedInts(c, r-1, int(1e9));

	Int(0, c-1);
	Space();
	Int(0, r-1);
	Endl();

	Int(0, c-1);
	Space();
	Int(0, r-1);
	Endl();
}
