#include "validator.h"


using namespace std;


void run() {
	int maxa=Arg("maxa",30);
	int maxb=Arg("maxb",30);
	int a = Int(0,maxa); Endl();
	int b = Int(0,maxb); Endl();

	Eof();
}
