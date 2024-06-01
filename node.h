//imports
#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <cstring>
using namespace std;

class Node {
public:
    //variables for nodes
    int data;
    Node* left;
    Node* right;
    Node* parent;
    char color; 
    //constructor and destructor 
    void moveDown(Node* newParent);
    //bool isLeftChild();
    Node(int val);
    ~Node();
    //no prototypes needed as no methods used in node.cpp
};

#endif // NODE_H
