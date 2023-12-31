
#ifndef PROBLEM2_H
#define PROBLEM2_H
#include "basicDS.h"

struct edgeList
{
    int index; // the index of edge in graph
    int dest; // the edge adjoint in
    int cost; // the cost of edge
};

struct Contain {
    int id; // the id of tree
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

struct Metric {
    int** distance;
    edgeList** edges;
};

#endif