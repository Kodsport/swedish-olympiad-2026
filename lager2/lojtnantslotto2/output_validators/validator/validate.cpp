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

string yes = "yes";
string no = "no";

void tolower_string(string &s){
    for(auto& c : s) c = tolower(c);
}

int main(int argc, char **argv) {
  init_io(argc, argv);

  int n;
  judge_in >> n;
  vi P(n, 0);
  rep(c1,0,n){
    judge_in >> P[c1];
    P[c1]--; 
  }
  
  string judge_row1;
  judge_ans >> judge_row1;
  tolower_string(judge_row1);
  bool judge_impossible = (judge_row1 == no);

  string row1;
  if(!(author_out >> row1)){
    wrong_answer("Could not read first row");
  }
  tolower_string(row1);

  if(row1 == no){
    if(!judge_impossible){
        wrong_answer("Claims impossible but judge found a solution");
    }
  }
  else{
    if(row1 != yes){
        wrong_answer("First line was not yes or no");
    }

    vector<string> grid;
    map<string,int> M;
    rep(c1,0,n){
        string s;
        if(!(author_out >> s)){
            wrong_answer("Could not read %dth string", c1+1);
        }
        if(s.length() != n){
            wrong_answer("Wrong length of %dth string", c1+1);
        }
        rep(c2,0,n){
            if(s[c2] != '0' && s[c2] != '1'){
                wrong_answer("Found invalid character in %dth string", c1+1);
            }
        }
        if(M.find(s) != M.end()){
            wrong_answer("Equal rows: %d and %d", M[s]+1, c1+1);
        }
        M[s] = c1;

        grid.push_back(s);
    }

    rep(c1,0,n){
        rep(c2,0,n){
            if(grid[c1][c2] != grid[c2][P[c1]]){
                wrong_answer("%dth string not equal to its column", c1+1);
            }
        }
    }

  }

  string garbage;
  author_out >> garbage;
  if (author_out)
    wrong_answer("trailing output, expected eof");

  accept();
}