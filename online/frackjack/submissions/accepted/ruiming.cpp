#include <bits/stdc++.h>

using namespace std;

int a,b;

int main(){
    cin>>a>>b;
    if(a>21){
        if(b>21){
            cout<<"Domare"<<'\n';
        }else{
            cout<<"Bertil"<<'\n';
        }
    }else{
        if(a>21){
            cout<<"Domare"<<'\n';
        }else{
            cout<<"Alice"<<'\n';
        }
    }
}