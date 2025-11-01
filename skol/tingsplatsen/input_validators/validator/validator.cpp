#include "validator.h"


using namespace std;


void run() {
	int n = Int(1, Arg("maxn", 10)); 
	Space();
	int m = Int(1, 10); 
	Endl();
	int houses = 0;
	for(int c1 = 0; c1 < n; c1++){
		string row = Line();
		assert(row.length() == m);
		for(int c2 = 0; c2 < m; c2++){
			assert(row[c2] == '.' || row[c2] == '*');
			if(row[c2] == '*')houses++;
		}
	}
	assert(houses >= 2);

	Eof();
}
