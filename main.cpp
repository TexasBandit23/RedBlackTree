//imports
#include <iostream>
#include <cstring>
#include "node.h"
#include <vector>
#include <fstream>
using namespace std;

//function prototypes
void add(Node*& node, int value);
void remove(Node*& node, int value);
bool search(Node* node, int value);
void print(Node* node, int count);
Node* findMin(Node* node);

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
	    add(root, temp); //add function call
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
	    add(root, *iter); //add function call
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
	remove(root, value); //delete function call
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
	print(root, 0); //print function call 
      }
    }
    
    return 0;
}

//add function
void add(Node*& node, int value) {
    //if a null node is found, add a new node with the taken in value there
    //if we reach a null node, we know we have gone through the tree
    
    if (node == nullptr){
        node = new Node(value);
	node->parent = nullptr;
    }
    else if(node->left == nullptr && value < node->data){
        node->left = new Node(value);
	node->left->parent = node;
	if (case2(node->left) == true){
	  
	}
	//cout << node->left->parent->data << endl;
    }
    else if(node->right == nullptr && value > node->data){
        node->right = new Node(value);
	node->right->parent = node;
	if (case2(node->right) == true){
	  
	}
	//cout << node->right->parent->data << endl;
    }
    //if the number being taken in is less than the node we are looking at, look at the left child and call add again
    else if (value < node->data){
        add(node->left, value);
    }
    //if the number being taken in is greater than the node we are looking at, look at the right child and call add again
    else if (value > node->data){
        add(node->right, value);
    }
}
void case1(Node*& node){
  if(node->parent == nullptr){
    node->color = 'b';
  }
  else{
    case2(node);
  }
}

void case2(Node* node){
  if(node->parent->color == 'b'){

  }
  else{
    case3(node);
  }
}
void case3(Node*& node){
  if(node->parent->parent->right->color == 'r' && node->parent->parent->left->color == 'r'){
    node->parent->parent->right->color = 'b';
    node->parent->parent->left->color = 'b';
    node->parent->parent->color = 'r';
    case1(node->parent->parent);
  }
  else{
    case4(node);
  }
}

void case4(Node*& node){
  if((node->parent->parent->left == node->parent) && (node->parent->right == node)){
    Node* parent = node->parent;
    Node* current = node;
    node->parent->parent->left = node;
    node->parent = node->parent->parent; 
    node->left = parent;
    
  }
}

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

//print function provided in class
//one thing to note is that if the tree is not balanced, it does not look good
//the tree will not print well, but, it does work
void print(Node* node, int count) {
    if(node == NULL){
      cout << "NOTHING IN THE TREE" << endl;
    return;
  }
  else if(node->right != NULL){
      print(node->right, count + 1);
    }

  for(int i = 0; i < count; i++){
        cout << '\t';
    }
    //prints
  cout << node->data << ": " << node->color << endl;

  if(node->left != NULL){
        print(node->left, count + 1);
    }
}
