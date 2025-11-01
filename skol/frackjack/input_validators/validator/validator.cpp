#include "validator.h"


using namespace std;


void run() {
	int maxa=Arg("maxa",30);
	int maxb=Arg("maxb",30);
	int a = Int(0,30); Endl();
	int b = Int(0,30); Endl();

	assert(a<=30);
	assert(b<=30);

	Eof();
}
