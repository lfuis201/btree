
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include "Btree.h"
#include <stdlib.h>
#include <random>
using namespace std;
void creartxt(){
    //random_device rd;
    //default_random_engine eng(rd());
    //uniform_real_distribution<float> distr(1, 1000000);
    srand(time(NULL));
    ofstream file;
    file.open("D:/programacion/c++/txt1gb/archivo.txt");
    long int a=100000;
    for (long int i = 0; i <a;i++){
        long int r= (rand() % 2147483647+1);
        file<< r <<"\n";
    }
    file.close();
}
void btree1(){
    long int cadena;
    ifstream fe("D:/programacion/c++/txt1gb/archivo.txt");
    BTree t(1000);
    while (!fe.eof()) {
        fe >> cadena;
        t.insert(cadena);
    }
    cout << "Traversal of the constructed tree is ";
    t.traverse();
}
int main()
{
    btree1();

    //borrar elemento
    //t.remove(6);

    //buscar un elemento
    //int k = 6;
    //(t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present";

    return 0;
}
