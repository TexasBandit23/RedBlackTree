/*
Author: Tejas Pandit
Last Edited: 5/2/2024
This program creates a Red-Black tree, a self balancing binary tree
The user can only insert numbers, print out the tree, and quit
*/

//imports
#include <iostream>
#include <cstring>
#include "node.h"
#include <vector>
#include <fstream>
using namespace std;

//function prototypes, commented out functions not used/needed
void add(Node*& node, int value, Node*& root);
//void remove(Node*& node, int value);
//bool search(Node* node, int value);
void print(Node* node, int count, Node* root);
//Node* findMin(Node* node);
void case1(Node*& node, Node*& root);
void case2(Node* node, Node*& root);
void case3(Node*& node, Node*& root);
void case4(Node*& node, Node*& root);
void case5(Node*& node, Node*& root);

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

    cout << "This is Binary Search Tree..." << endl;

    //run while loop until "QUIT" is entered
    while(running == true){
      //ask for user input
      cout << "Please enter a command: ADD, PRINT, or QUIT" << endl;
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
      /*
      //if DELETE
      //function not used
      else if(strcmp(command, "DELETE") == 0){
	//user input
	cout << "Which value would you like to delete?" << endl;
	cin >> value;
	cin.get();
	//delete user inputted number
	remove(root, value); //delete function call
      }
      //if SEARCH
      //function not used
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
      */
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
/*
//this function is used to find the "next larger"
//it iterates left until it reaches the leftmost child
Node* findMin(Node* node) {
    while (node->left != nullptr)
        node = node->left;
    return node;
}

//remove function
void remove(Node*& node, int value) {
    //if we have gone through the tree and the value has not been found, it does not exist and there is nothing to remove  
    if (node == nullptr)
        return;
    //if the value is less than the value we are looking at, look to the left and call remove again
    if (value < node->data)
        remove(node->left, value);
    //if the value is greater than the value we are looking at, look to the right and call remove again
    else if (value > node->data)
        remove(node->right, value);
    //if this statement is reached, we have found the value we would like to remove
    else {
        //if the node has no children, simply remove
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
	//if the node has only a right child remove the node by setting its value to be that of the child     
        } else if (node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
	//if the node has only a left child remove the node by setting its value to be that of the child
        } else if (node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
	//this is the case of the node being removed has two children
	} else {
	    //use findMin to find the next larger - go one to the right then all the way to the left
            Node* temp = findMin(node->right);
            node->data = temp->data; //set the value of the node being removed to that of the next larger
            remove(node->right, temp->data); //call remove again to remove the next larger node
        }
    }
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
*/

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
    cout << node->data << ": " << node->color << ", Parent: N/A" << endl; //if the node is the root, it has no parent so, print out N/A
  }
  else{
    cout << node->data << ": " << node->color << ", Parent: " << node->parent->data << endl; //print out value, color, and parent
  }
  
  if(node->left != NULL){
    print(node->left, count + 1, root);
    }
}
