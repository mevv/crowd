#ifndef LEEPATHFINDER_H
#define LEEPATHFINDER_H

#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>
#pragma once

#define Coord pair<int, int>
#define CoordVector vector<pair<int, int> >

using namespace std;

class LeePathfinder
{
    int *field;

    Coord start;
    Coord end;

    int size_x;
    int size_y;


    int *indexes;

    CoordVector path;

    void Backtrace();


    bool MarkPoint(int i, int j, int n); // Marking points on wave expansion

public:
    LeePathfinder(int *field, int size_x, int size_y);

    CoordVector* CalculatePath();
    int *GetIndexedMap() { return indexes; }
    void PrintMap();
    ~LeePathfinder();
};


#endif // LEEPATHFINDER_H
