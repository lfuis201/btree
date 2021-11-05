
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <assert.h>
#include "Btree.h"
using namespace std;
void creartxt(){
    srand(time(NULL));
    ofstream file;
    file.open("D:/programacion/c++/txt1gb/archivo.txt");
    int a=126;
    for (int i = 0; i <a;i++){
        int r = (rand() % 1024);
        file<< r <<"\n";
    }
    file.close();
}
int main()
{
    long cadena;
    ifstream fe("D:/programacion/c++/txt1gb/archivo.txt");
    BTree t(3);
    while (!fe.eof()) {
        fe >> cadena;
        t.insert(cadena);
    }
    {



        cout << "Traversal of the constructed tree is ";
        t.traverse();
    }

    return 0;

}
