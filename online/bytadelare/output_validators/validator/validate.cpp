#include "validate.h"

#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;

string ja = "ja";
string nej = "nej";

void tolower_string(string &s){
    for(auto& c : s) c = tolower(c);
}

int main(int argc, char **argv) {
  init_io(argc, argv);

  int n,x;
  judge_in >> n >> x;
  vi A(n, 0);
  rep(c1,0,n){
    judge_in >> A[c1]; 
  }
  
  string judge_row1;
  judge_ans >> judge_row1;
  tolower_string(judge_row1);
  bool judge_impossible = (judge_row1 == nej);

  string row1;
  if(!(author_out >> row1)){
    wrong_answer("Could not read first row");
  }
  tolower_string(row1);

  if(row1 == nej){
    if(!judge_impossible){
        wrong_answer("Claims impossible but judge found a solution");
    }
    if(x == 0)accept();
  }
  else{
    if(judge_impossible && x == 0){
        wrong_answer("Says possible, judge did not find solution, X = 0");
    }
    if(!judge_impossible && x == 0)accept();

    int m;
    if(!(author_out >> m)){
        wrong_answer("Could not read m");
    }
    if(m < 0 || m > 1000000){
        wrong_answer("m out of range %d", m);
    }

    vi A2 = A;
    sort(all(A2));
    rep(c1,0,m){
        int i,j;
        if(!(author_out >> i >> j)){
            wrong_answer("Could not read i and j on row %d", c1+1);
        }
        if(i < 1 || i > n || j < 1 || j > n){
            wrong_answer("i,j out of range on row %d", c1+1);
        }
        i--;
        j--;
        if(!(A[i]%A[j] == 0 || A[j]%A[i] == 0)){
            wrong_answer("Incorrect move, attempts to swap non-divisors, row %d", c1+1);
        }
        swap(A[i], A[j]);
    }

    if(A != A2)wrong_answer("Did not sort list.");

    if(judge_impossible){
        judge_error("Contestant found solution but judge said impossible");
    }

  }

  string garbage;
  author_out >> garbage;
  if (author_out)
    wrong_answer("trailing output, expected eof");

  accept();
}