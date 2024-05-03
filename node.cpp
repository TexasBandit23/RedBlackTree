//imports
#include "node.h"
#include <iostream>
#include <cstring>
using namespace std;

//constructor 
Node::Node(int val){
  data = val;
  left = nullptr;
  right = nullptr;
  parent = nullptr;
  color = 'r'; //nodes are inserted as red
}

//destructor
Node::~Node(){
  
}
//decided to not code getters and setters as they are not needed
//we can just use node->data, left, or right instead
