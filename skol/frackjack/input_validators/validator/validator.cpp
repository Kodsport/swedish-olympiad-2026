#include "validator.h"


using namespace std;


void run() {
	int n = Int(1, Arg("maxn", int(1e9))); Endl();
	int m = Int(2, 9); Endl();

	int m_is = Arg("m",-1);
	if (m_is!=-1) assert(m==m_is);

	Eof();
}
