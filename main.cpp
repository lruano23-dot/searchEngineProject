/*------------------------------------
Program 2: Search Engine Project
Course: CS 251, Fall 2023, UIC
System: VSCode
Author: Lisette Ruano
--------------------------------------*/

#include "search.h"
using namespace std;

int main() {

    string filename;
    getline(cin, filename);
    searchEngine(filename);

    //runTests();
    
    return 0;
}