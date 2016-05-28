#include <iostream>

using namespace std;

void error(int e, int n)
{
    if (e==100) cout << "["<< n << "]" <<" You have "<<e<<" error" << endl << "General error in syntax";
    if (e==101 || e==110) cout << "["<< n << "]" <<" You have "<<e<<" error" << endl << "Error in create table syntax";
    if (e==102 || e==103 || e==104) cout << "["<< n << "]" <<" You have "<<e<<" error" << endl << "Error in delete syntax";
    if (e==106) cout << "["<< n << "]" <<" You have "<<e<<" error" << endl << "Error in insert syntax";
    if (e==107) cout << "["<< n << "]" <<" You have "<<e<<" error" << endl << "Error in copy syntax";
    if (e==108) cout << "["<< n << "]" <<" You have "<<e<<" error" << endl << "Error in sort syntax";
    if (e==109) cout << "["<< n << "]" <<" You have "<<e<<" error" << endl << "Error in select syntax";

    cout<<"lol";
}
