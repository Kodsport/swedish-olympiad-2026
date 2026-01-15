#include <bits/stdc++.h>

using namespace std;

int main(){
	int n, m, q; cin >> n >> q;
	vector<int> v[500001];

	for (int i = 0; i < n; i++){
		int k; cin >> k;
		for (int j = 0; j < k; j++){
			int temp; cin >> temp;
			v[i].push_back(temp);
		}
	}


	while (q--){
		int k; cin >> k;
		int maxi = -1, ret = -1;
		for (int i = 0; i < n; i++){
			int amount = 0;
			for (int x : v[i])amount += (x >= k);
			if (amount >= maxi){
				maxi = amount;
				ret = i+1;
			}
		}
		cout << ret << endl;
	}

	return 0;
}
