/*
Author: Tejas Pandit
Last Edited: 5/22/2024
This program creates a Red-Black tree, a self balancing binary tree
The user can insert and delete numbers, print out the tree, search for numbers in the tree, and quit
*/

//imports
#include <iostream>
#include <cstring>
#include "node.h"
#include <vector>
#include <fstream>
using namespace std;

//function prototypes
void add(Node*& node, int value, Node*& root);
void remove(Node*& node, int value, Node*& root);
bool search(Node* node, int value);
void print(Node* node, int count, Node* root);
Node* findMin(Node* node);
void case1(Node*& node, Node*& root);
void case2(Node* node, Node*& root);
void case3(Node*& node, Node*& root);
void case4(Node*& node, Node*& root);
void case5(Node*& node, Node*& root);
//old prototype that didn't work
//void replaceNode(Node*& oldNode, Node*& newNode, Node*& root);
void fixDoubleBlack(Node*& root, Node* node);
Node* getSibling(Node* node);
bool isLeftChild(Node* node);
bool hasRedChild(Node* node);
//old prototype that didn't work
//void fixDelete(Node*& root, Node*& node);
void rotateLeft(Node*& root, Node*& node);
void rotateRight(Node*& root, Node*& node);

int main() {
    //initialize variables
    Node* root = nullptr;
    bool running = true;
    char command[50];
    char input[50];
    char cf;
    int numOfNums;
    int value;
    int temp;

    cout << "This is Red-Black Tree..." << endl;

    //run while loop until "QUIT" is entered
    while(running == true){
      //ask for user input
      cout << "Please enter a command: ADD, DELETE, SEARCH, PRINT, or QUIT" << endl;
      //get a command from the user
      cin.get(command, 50);
      cin.get();
      //if ADD
      if(strcmp(command, "ADD") == 0){
	//ask if console or file
	cout << "Would you like to enter in the console (c) or by a file (f)?" << endl;
	cin >> cf;
	cin.get();
	//read in from console
	if(cf == 'c'){
	  cout << "How many numbers are you adding?" << endl;
	  cin >> numOfNums;
	  cin.get();
	  cout << "Enter your numbers: " << endl;
	  //allows for multiple numbers to be inputted by user at once
	  for (int i = 0; i < numOfNums; i++){
	    cin >> temp;
	    cin.get();
	    add(root, temp, root); //add function call
	  }
	}
	//read in from file
	else if(cf == 'f'){
	  vector<int> numbers;
	  fstream first_stream;
	  first_stream.open("numbers.txt");
	  int number;
	  while(first_stream >> number){
	    numbers.push_back(number);
	  }
	  vector <int> ::iterator iter = numbers.begin();
	  for (iter = numbers.begin(); iter < numbers.end(); iter++){
	    add(root, *iter, root); //add function call
	  }
	}
      }
      
      //if DELETE
      else if(strcmp(command, "DELETE") == 0){
	//user input
	cout << "Which value would you like to delete?" << endl;
	cin >> value;
	cin.get();
	//delete user inputted number
	remove(root, value, root); //delete function call
      }
      //if SEARCH
      else if(strcmp(command, "SEARCH") == 0){
	//user input
	cout << "What value would you like to search for?" << endl;
	cin >> value;
	cin.get();
	//search function call, if the value exists say it does, if it does not exists say it is not present
	if(search(root, value) == true){
	  cout << "This value exists..." << endl;
	}
	else if(search(root, value) == false){
	  cout << "This value does not exist..." << endl;
	}
      }
      
      //if QUIT
      else if(strcmp(command, "QUIT") == 0){
	//set running to false to end the while loop
	cout << "Goodbye..." << endl;
	running = false;
      }
      //if PRINT
      else if(strcmp(command, "PRINT") == 0){
	print(root, 0, root); //print function call 
      }
    }
    
    return 0;
}

//add function
void add(Node*& node, int value, Node*& root) {
    //if a null node is found, add a new node with the taken in value there
    //if we reach a null node, we know we have gone through the tree
    //this add function makes what could be considered a doubly linked list tree as each node will point to its parent and children
    
    if (node == nullptr){
        node = new Node(value);
	node->parent = nullptr;
	case1(node, root); //a node has been inserted, call cases to adjust
    }
    else if(node->left == nullptr && value < node->data){
        node->left = new Node(value);
	node->left->parent = node;
	case1(node->left, root); //a node has been inserted, call cases to adjust
	//cout << node->left->parent->data << endl;
    }
    else if(node->right == nullptr && value > node->data){
        node->right = new Node(value);
	node->right->parent = node;
	case1(node->right, root); //a node has been inserted, call cases to adjust
	//cout << node->right->parent->data << endl;
    }
    //if the number being taken in is less than the node we are looking at, look at the left child and call add again
    else if (value < node->data){
      add(node->left, value, root);
    }
    //if the number being taken in is greater than the node we are looking at, look at the right child and call add again
    else if (value > node->data){
      add(node->right, value, root);
    }
}
void case1(Node*& node, Node*& root){
  if(node->parent == nullptr){ //if the node we are looking at is the root, change it to black and we are done
    node->color = 'b';
  }
  else{
    case2(node, root); //if we are not looking at root, move to next case
  }
}

void case2(Node* node, Node*& root){
  if(node->parent->color == 'b'){ //if the parent of the node we are looking at is black, we are done

  }
  else{
    case3(node, root); //if parent is not black, call next case
  }
}
void case3(Node*& node, Node*& root){
  if((node->parent->parent->right == nullptr) || (node->parent->parent->left == nullptr)){
    case4(node, root); //call next case
  }
  //parent and uncle are both red, grandparent is black, switch these
  else if(node->parent->parent->right->color == 'r' && node->parent->parent->left->color == 'r'){
    node->parent->parent->right->color = 'b';
    node->parent->parent->left->color = 'b';
    node->parent->parent->color = 'r';
    case1(node->parent->parent, root); //recursive case 1 call on grandparent
  }
  else{
    case4(node, root); //call next case
  }
}

void case4(Node*& node, Node*& root){
  //node rotation through parent
  if((node->parent->parent->left == node->parent) && (node->parent->right == node)){
    Node* parent1 = node->parent;
    Node* current1 = node;

    //create new nodes
    Node* newNode1 = new Node(current1->data);
    Node* newParent1 = new Node(parent1->data);

    //connect nodes and complete rotation
    if(current1->left != nullptr){
      newParent1->right = current1->left;
      newParent1->right->parent = newParent1;
    }

    if(parent1->left != nullptr){
      newParent1->left = parent1->left;
      newParent1->left->parent = newParent1;
    }

    if(newNode1->right != nullptr){
      newNode1->right = current1->right;
      newNode1->right->parent = newNode1;
    }
    
    newNode1->left = newParent1;
    newParent1->parent = newNode1;

    //connect new nodes with grandparent completing rotation
    node->parent->parent->left = newNode1;
    newNode1->parent = node->parent->parent;

    case5(newParent1, root); //call next case
  }
  //node rotation through parent, same as above but inverted
  else if((node->parent->parent->right == node->parent) && (node->parent->left == node)){
    Node* parent2 = node->parent;
    Node* current2 = node;

    Node* newNode2 = new Node(current2->data);
    Node* newParent2 = new Node(parent2->data);

    if(current2->right != nullptr){
      newParent2->left = current2->right;
      newParent2->left->parent = newParent2;
    }

    if(parent2->right != nullptr){
      newParent2->right = parent2->right;
      newParent2->right->parent = newParent2;
    }

    if(newNode2->left != nullptr){
      newNode2->left = current2->left;
      newNode2->left->parent = newNode2;
    }

    newNode2->right = newParent2;
    newParent2->parent = newNode2;

    node->parent->parent->right = newNode2;
    newNode2->parent = node->parent->parent;

    case5(newParent2, root); //call next case
  }
  else{
    case5(node, root); //no rotation needed, call next case
  }
}

void case5(Node*& node, Node*& root){
  //rotation through grandparent
  if(node->parent->left == node){
    Node* current3 = node;
    Node* parent3 = node->parent;
    Node* grandparent3 = node->parent->parent;
    Node* uncle3 = node->parent->parent->right;

    //create new nodes
    Node* newParent3 = new Node(parent3->data);
    Node* newGrandparent3 = new Node(grandparent3->data);

    //maintain connections and complete rotation
    if(parent3->right != nullptr){
      newGrandparent3->left = parent3->right;
      newGrandparent3->left->parent = newGrandparent3;
    }

    if(uncle3 != nullptr){
      newGrandparent3->right = uncle3;
      newGrandparent3->right->parent = newGrandparent3;
    }
    
    newParent3->left = current3;
    newParent3->left->parent = newParent3;

    //connect new and old nodes to complete rotation
    newParent3->right = newGrandparent3;
    newParent3->right->parent = newParent3;

    newParent3->parent = grandparent3->parent;

    //if grandparent had a parent, connect this to the parent
    if(grandparent3->parent != nullptr){
      if(grandparent3->parent->right == grandparent3){
	grandparent3->parent->right = newParent3;
      }
      else if(grandparent3->parent->left == grandparent3){
	grandparent3->parent->left = newParent3;
      }
    }
    //if the parent is now the root, make the change
    else if(grandparent3->parent == nullptr){
      root = newParent3;
    }
    //change colors
    newGrandparent3->color = 'r';
    newParent3->color = 'b';
  }
  //rotation through grandparent, same as above but inverted
  else if(node->parent->right == node){
    Node* current4 = node;
    Node* parent4 = node->parent;
    Node* grandparent4 = node->parent->parent;
    Node* uncle4 = node->parent->parent->left;

    Node* newParent4 = new Node(parent4->data);
    Node* newGrandparent4 = new Node(grandparent4->data);

    if(parent4->left != nullptr){
      newGrandparent4->right = parent4->left;
      newGrandparent4->right->parent = newGrandparent4;
    }

    if(uncle4 != nullptr){
      newGrandparent4->left = uncle4;
      newGrandparent4->left->parent = newGrandparent4;
    }

    newParent4->right = current4;
    newParent4->right->parent = newParent4;

    newParent4->left = newGrandparent4;
    newParent4->left->parent = newParent4;

    if(grandparent4->parent != nullptr){
      newParent4->parent = grandparent4->parent;
      if(grandparent4->parent->right == grandparent4){
        grandparent4->parent->right = newParent4;
      }
      else if(grandparent4->parent->left == grandparent4){
        grandparent4->parent->left = newParent4;
      }
    }
    else if(grandparent4->parent == nullptr){
      root = newParent4;
    }
    newGrandparent4->color = 'r';
    newParent4->color = 'b';
  }
}
//iterates left until it reaches the leftmost child
Node* findMin(Node* node) {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

//remove function
void remove(Node*& root, int value, Node*& rootRef) {
    Node* nodeToDelete = root;
    Node* nodeToReplace;

    //find the node to delete
    while (nodeToDelete != nullptr && nodeToDelete->data != value) {
        if (value < nodeToDelete->data) {
            nodeToDelete = nodeToDelete->left;
        } else {
            nodeToDelete = nodeToDelete->right;
        }
    }

    if (nodeToDelete == nullptr) {
        return; //node not found
    }

    //if node has two children, we need the in-order successor
    if (nodeToDelete->left != nullptr && nodeToDelete->right != nullptr) {
        Node* successor = findMin(nodeToDelete->right); //smallest value in right tree
        nodeToDelete->data = successor->data; //replace data with successor data
        nodeToDelete = successor; //we now need to delete the successor
    }

    //determine what will repalce the node to be deleted
    nodeToReplace = (nodeToDelete->left != nullptr) ? nodeToDelete->left : nodeToDelete->right;

    //if the node to be deleted has a child
    if (nodeToReplace != nullptr) {
        nodeToReplace->parent = nodeToDelete->parent;
	//node is root
	if (nodeToDelete->parent == nullptr) {
            rootRef = nodeToReplace;
	    //node is left child
        } else if (nodeToDelete == nodeToDelete->parent->left) {
            nodeToDelete->parent->left = nodeToReplace;
	    //node is right child
	} else {
            nodeToDelete->parent->right = nodeToReplace;
        }

	//fix double black issue
        if (nodeToDelete->color == 'b') {
            fixDoubleBlack(rootRef, nodeToReplace);
        }
	//delete node
        delete nodeToDelete;

	//node is root and has no children
    } else if (nodeToDelete->parent == nullptr) {
        rootRef = nullptr;
        delete nodeToDelete;
	//node to delete is a leaf
    } else {
        //if the leaf is black, issue
        if (nodeToDelete->color == 'b') {
            fixDoubleBlack(rootRef, nodeToDelete);
        }
	//update parent
        if (nodeToDelete->parent != nullptr) {
            if (nodeToDelete == nodeToDelete->parent->left) {
                nodeToDelete->parent->left = nullptr;
            } else {
                nodeToDelete->parent->right = nullptr;
            }
        }
	//delete
        delete nodeToDelete;
    }
}

void fixDoubleBlack(Node*& root, Node* node) {
    //node is root, nothing to be done
    if (node == root) {
        return;
    }

    //get sibling and parent
    Node* sibling = getSibling(node);
    Node* parent = node->parent;

    //if sibling is null, fix issue at parent level
    if (sibling == nullptr) {
        fixDoubleBlack(root, parent);
    } else {
        //if sibling is red
        if (sibling->color == 'r') {
	  parent->color = 'r'; //color parent red
	  sibling->color = 'b'; //color sibling black
	    //rotations
	    if (isLeftChild(sibling)) {
                rotateRight(root, parent);
            } else {
                rotateLeft(root, parent);
            }
	    //fix issue at original node
            fixDoubleBlack(root, node);
	//sibling is black but has a red child
        } else {
            if (hasRedChild(sibling)) {
	        //determine which sibling is red
                if (sibling->left != nullptr && sibling->left->color == 'r') {
		  //sibling is left child and its left child is red
		    if (isLeftChild(sibling)) {
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        rotateRight(root, parent);
		    //sibling is right child and its left child is red
		    } else {
                        sibling->left->color = parent->color;
                        rotateRight(root, sibling);
                        rotateLeft(root, parent);
                    }
                } else {
		    //sibling is left child and its right child is red
                    if (isLeftChild(sibling)) {
                        sibling->right->color = parent->color;
                        rotateLeft(root, sibling);
                        rotateRight(root, parent);
                    } else {
		      //sibling is right child and its right child is red
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        rotateLeft(root, parent);
                    }
                }
		//color parent
                parent->color = 'b';
            } else {
	      //sibling is black and has no children
                sibling->color = 'r';
		//if parent is also black, fix issue at parent
                if (parent->color == 'b') {
                    fixDoubleBlack(root, parent);
                } else {
                    parent->color = 'b';
                }
            }
        }
    }
}

Node* getSibling(Node* node) {
    //if the node has no parent, it has no sibling
    if (node->parent == nullptr) {
        return nullptr;
    }
    //if the node is a left child, the parent's right child is the sibling
    if (isLeftChild(node)) {
        return node->parent->right;
    } else {
      //the node is a right child so, the sibling is the parent's left child
        return node->parent->left;
    }
}

//if the node is the left child of its parent, returns true
bool isLeftChild(Node* node) {
    return node == node->parent->left;
}

//if either child is red, return true
bool hasRedChild(Node* node) {
    return (node->left != nullptr && node->left->color == 'r') || (node->right != nullptr && node->right->color == 'r');
}

//performs a rotation left
void rotateLeft(Node*& root, Node*& node) {
    Node* newParent = node->right;
    if (node == root) {
        root = newParent;
    }
    node->moveDown(newParent);
    node->right = newParent->left;
    if (newParent->left != nullptr) {
        newParent->left->parent = node;
    }
    newParent->left = node;
}

//performs a rotation right
void rotateRight(Node*& root, Node*& node) {
    Node* newParent = node->left;
    if (node == root) {
        root = newParent;
    }
    node->moveDown(newParent);
    node->left = newParent->right;
    if (newParent->right != nullptr) {
        newParent->right->parent = node;
    }
    newParent->right = node;
}

void Node::moveDown(Node* newParent) {
    //check if current node has a parent
    if (parent != nullptr) {
      //if the node is left child, update "left" of parent 
        if (isLeftChild(this)) {
            parent->left = newParent;
        } else {
	  //node is right right child so update "right" of parent
            parent->right = newParent;
        }
    }
    //update
    newParent->parent = parent;
    parent = newParent;
}

//search function
bool search(Node* node, int value) {
    //if we go through the tree and the value is not found, it does not exist
    if (node == nullptr)
        return false;
    //if we found the node, say that it does exist
    if (node->data == value)
        return true;
    //if the value is less than the value we are looking at, look to the left and call search again
    if (value < node->data)
        return search(node->left, value);
    //if the value is greater than the value we are looking at, look to the right and call search again
    else
        return search(node->right, value);
}


//print function provided in class
void print(Node* node, int count, Node* root) {
    if(node == NULL){
      cout << "NOTHING IN THE TREE" << endl;
    return;
  }
  else if(node->right != NULL){
    print(node->right, count + 1, root);
    }

  for(int i = 0; i < count; i++){
        cout << '\t';
    }
    //prints
  if(node == root){
    cout << node->data << ": " << node->color << endl; //if the node is the root, it has no parent so, print out N/A
  }
  else{
    cout << node->data << ": " << node->color << endl; //print out value, color, and parent
  }
  
  if(node->left != NULL){
    print(node->left, count + 1, root);
    }
}
