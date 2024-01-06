
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

template <class T>
class Node {
    friend class Linklist;
    private:
        T* data;
        Node<T>* next;
    public:
        Node(): data(nullptr), next(nullptr){};
        Node(T datat): data(datat), next(nullptr){};
};

template <class T>
class Linklist {
    private:
        Node<T>* head;
        Node<T>* tail;
        int size;
    public:
        Linklist(): head(nullptr), tail(nullptr), size(0) {};

        void insert(T data) {
            if (tail) {
                Node<T> temp; 
            }
        }
};
#endif