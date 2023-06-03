#include <iostream>
#include "Dijakrsta.h"
using namespace std;

void insert(int& row, int& column, char sign) {
    cout << endl;
    cout << "COORDINATES OF POINT " << sign << ":" << endl;
    cout << "Row(" << sign << "): ";
    cin >> row;
    cout << "Column(" << sign << "): ";
    cin >> column;
}

int main()
{
    // system("color cyan"); // for checking color atributes
    system("color 5f");

    bool isWalls;
    cout << "Run the simulation with walls or without them (y/1, n/0): ";
    cin >> isWalls;

    cout << "Range of rows is [1, 20]." << endl;
    cout << "Range of columns is [1, 40]." << endl;
    cout << "Run again if nothing happens." << endl;

    int rowA;
    int columnA;
    insert(rowA, columnA, 'A');
    
    int rowB;
    int columnB;
    insert(rowB, columnB, 'B');

    Dijakrsta path(isWalls, rowA, columnA, rowB, columnB);

    return 0;
}
