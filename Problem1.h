
#ifndef PROBLEM1_H
#define PROBLEM1_H
#include "basicDS.h"

struct edgeList
{
    int index; // the index of edge in graph
    int dest; // the edge adjoint in
    int cost; // the cost of edge
};

struct Contain {
    vector<int> vertex; // the index of the vertex
};

bool Compare(edgeList left, edgeList right) {
    return left.cost < right.cost;
}

struct cmp {
    bool operator()(const edgeList left,const edgeList right) {
        return left.cost > right.cost;
    }
};

struct MyTree
{
    Tree MTid;
    Contain usage;
    int t;
};

#endif