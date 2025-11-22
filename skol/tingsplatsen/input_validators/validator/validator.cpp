#include "validator.h"

#define rep(i,a,b) for(int i = (a); i < (b); i++)

using namespace std;


void run() {
	int n = Int(1, Arg("maxn", 10)); 
	Space();
	int m = Int(1, 10); 
	Endl();
	int houses = 0;
	vector<string> grid;
	for(int c1 = 0; c1 < n; c1++){
		string row = Line();
		grid.push_back(row);
		assert(row.length() == m);
		for(int c2 = 0; c2 < m; c2++){
			assert(row[c2] == '.' || row[c2] == '*');
			if(row[c2] == '*')houses++;
		}
	}
	assert(houses >= 2);


	bool hasatleastone = false;
	rep(i,0,n) {
        rep(j,0,m) {
            if (grid[i][j] == '.') {
                int dist = 0;

                rep(x,0,n) rep(y,0,m) {
                    if (grid[x][y] == '*') {
                        if (dist == 0) {
                            dist = abs(x-i) + abs(y-j);
                        }

                        else {
                            if (dist != abs(x-i) + abs(y-j)) dist = -1;
                        }
                    }
                }

                if (dist != -1) {
                    hasatleastone = true;
                }
            }
        }
    }

	assert(hasatleastone);

	Eof();
}
