#pragma once
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<iomanip>
#include<math.h>
#include<queue>
#include<vector>

using namespace std;
template <typename T1, typename T2>
class Node
{
	private:
		T1 key;					//city name
		vector<T2> value;		//list of hotels in the city
		Node<T1,T2>* left;		//left child
		Node<T1,T2>* right;		//right child
		Node<T1,T2>* parent;	//pointer to the parent node
	public:
		Node(T1 key): key(key),left(nullptr),right(nullptr), parent(nullptr) 
		{}
		void print(string stars="")	//print all or only those hotels with specific stars.
		{
			int counter=0;
			cout<<std::left<<setw(50)<<"Name"<<" "<<setw(10)<<"City"<<" "<<setw(5)<<"Stars"<<" "<<setw(10)<<"Price"<<" "<<setw(20)<<"Country"<<" "<<"Address"<<endl;
			cout<<"------------------------------------------------------------------------------------------------------------"<<endl;
			for(auto it:value){
				if(stars=="" or stars==it->getStars())
				{
					it->print2(); //use print funciton from hotel
					counter++;
				}
			}
			
			cout<<"------------------------------------------------------------------------------------------------------------"<<endl;
			cout<<counter<<" records found."<<endl;
		}

		template<typename,typename> friend class BST;
		friend class HashTable; //add hashtable class as a friend 
};
//=============================================================================
template <typename T1, typename T2>
class BST //AVL IMPLEMENTATION 
{
	private:
		Node<T1,T2> *root; //root of bst
	public:
		BST();									//constructor
		~BST();									//destructor
		Node<T1,T2>* getRoot();						//returns the root of the Tree
		void insert(Node<T1,T2>* ptr,T1 key, T2 value);			//Insert key into tree/subtree with root ptr
		int height(Node<T1,T2> *ptr);				    //Find the height of a tree/subtree with root ptr
		Node<T1,T2>* find(Node<T1,T2> *ptr,T1 key,string stars);		//Find and returns the node with key
		Node<T1,T2>* findMin(Node<T1,T2> *ptr);				//Find and return the Node<T1,T2> with minimum key value from a tree/subtree with root ptr
		void findPrint(T1 key,string star);    //
		int balanceFactor(Node<T1,T2> *ptr); //calculate balance factor of a node
		void rotateRight(Node<T1,T2> *z); //trinode restructuring 
		void rotateLeft(Node<T1,T2> *z); //trinode restructuring 
		void rebalance(Node<T1,T2> *ptr);//trinode restructuring 
		void removeNode(Node<T1,T2> *ptr, T1 key,vector<T2> value); //remove node method for destructor
		void remove(Node<T1,T2> *ptr,T1 key, T2 value);			//Remove a node with key from the tree/subtree with root
		friend class HashTable; 
};
//=====================================================================
//write implementation of all methods below

template <typename T1, typename T2> BST<T1,T2>::BST(){ //constructor for bst
	this->root=nullptr; //set root to null
}
template <typename T1, typename T2> BST<T1,T2>::~BST(){ //destructor
	while(root) //remove all nodes
		removeNode(root,root->key, root->value);

}
template <typename T1, typename T2> Node<T1,T2>* BST<T1,T2>::getRoot(){ //getter function for root
	return root; 
}
template <typename T1, typename T2> void BST<T1,T2>:: removeNode(Node<T1,T2> *ptr, T1 key,vector<T2> value){
	if (ptr == nullptr)				//Node Not Found		
	{
		cout<<"Key not found"<<endl;	//Reached to bottom of the tree but key not found
		return;
	}
	if(key<ptr->key) 						//if key is smaller than the key at root of the tree/subtree
		removeNode(ptr->left, key, value);		//then search in left subtree
	else if(key>ptr->key)
		removeNode(ptr->right, key, value);	//else search in right subtree
	else
	{
		Node<T1, T2>* node = ptr;
		Node<T1, T2>* nextLink; 

		if( node->left != nullptr && node->right != nullptr)			//if the node to be deleted has both left and right children
		{
			Node<T1, T2> *min = findMin(node->right);//find the node with minimum key in its right subtree 
			node->key = min->key;
			removeNode(node->right, min->key, min->value);	//recursivly remove the minimum node from right subtree
			rebalance(ptr);
			return;
		}	
		else if(node->left == nullptr && node->right == nullptr)		//if the node to be deleted is a Leaf node then remove it
			nextLink = nullptr;					//parent will be connected to nullptr
		else if(node->right == nullptr)			//if the node to be deleted has no right child then remove it and connect its left child to its parent
		{
			node->left->parent = node->parent;	//connect left child to parent			
			nextLink = node->left;				//parent will be connected to left child
		}
		else if(node->left == nullptr)			//if the node to be deleted has no left child then remove it and connect its right child to its parent
		{
			node->right->parent = node->parent;	//connect right child to the parent
			nextLink = node->right;				//parent will be connected to right child
		}
			
		Node<T1, T2>* parent=node->parent;				
		if(parent==nullptr)				root = nextLink;			//in the parent is nullptr, then root is going to be deleted, update the root i.e.connect it to next link
		else if(node==parent->left)		parent->left = nextLink;	//if node to be deleted is left child of the parent then updated parents left link
		else							parent->right = nextLink;	//if node to be deleted is right child of the parent then update parents right link 
		if (ptr==node) ptr=nullptr;		//if the node for which the remove is called has been deleted, set it to null ptr so that loadFactor will not generate a seg. fault when called by rebalance
		delete node;		//delete the node
	}
	rebalance(ptr);	
}
template <typename T1, typename T2> void BST<T1,T2>::insert(Node<T1,T2>* ptr,T1 key, T2 value){
	if (root == nullptr) { //if there is no root
		root= new Node<T1,T2>(key); //the inserted node becomes the new root
		vector<T2> v = root->value;//create vector for node
		v.push_back(value); //add value to vector
	}  
	else if(key == ptr->key){	 //if there is a duplicate found
	//check if there is a hotel with the same name and city
		bool updated = false; 
		for(int i =0; i<ptr->value.size(); i++){ //iterate through vector
			if(ptr->value.at(i)->getName()==value->getName()){ //if hotel name matches
				ptr->value.at(i) = value;  //update pre-existing details of hotel
				updated = true; 
			}
		}
		if(updated ==false){ //if there was no duplicate
			ptr->value.push_back(value); //add to vector
			return;
		}
		
		if(updated == true){//there was a duplicate
			cout<<"Existing record has been updated."<<endl; //print confirmation message
			return;
		}

	}
	else if (key>ptr->key){ //key of new node is greater than the key of current node
		if (ptr->right ==nullptr)
		{
		    ptr->right = new Node<T1,T2>(key);
			ptr->right->value.push_back(value); 
		    ptr->right->parent = ptr;
		}
		else
			insert(ptr->right ,key,value); //recursive insert 
	}
  
    else if (key < ptr->key){//key of new node is smaller than the key of current node
		if (ptr->left ==nullptr)
		{
		    ptr->left = new Node<T1,T2>(key);
			ptr->left->value.push_back(value);
		    ptr->left->parent = ptr;
		}
		else
			insert(ptr->left,key,value); //recursive insert 
	}
}
template <typename T1, typename T2> int BST<T1,T2>::height(Node<T1,T2> *ptr){
	if(ptr==nullptr) return -1; //height of an empty tree/subtree is considered to be -1
		return max(height(ptr->left),height(ptr->right))+1;
}
template <typename T1, typename T2> Node<T1,T2>* BST<T1,T2>::find(Node<T1,T2> *ptr,T1 key,string stars){
	bool found =false;
	if(ptr==nullptr){
		return nullptr;					//No key found
	} 
	else if(key < ptr->key){
		return find(ptr->left,key,stars);	//search in the left subtree
	}
	else if(key >ptr->key){
		return find(ptr->right,key,stars); //search in the right subtree
	} 
	else if(ptr->key ==key && stars!=""){ //if key is found and the user for stars
		for(int i =0; i<ptr->value.size(); i++){ //iterate through vector
			if(ptr->value.at(i)->getStars() == stars){ //if stars match
				found ==true; //found variable set to true 
				return ptr;  //return node  
			}
		}
	}
	else if(key==ptr->key && stars==""){ //if user is not looking for stars
		found == true; //found set to true
		return ptr; //return node
	}
	if(found == false){ //if node not found, return nullptr
		return nullptr; 
	}
}
template <typename T1, typename T2> void BST<T1,T2>::findPrint(T1 key,string stars){ //find given hotel and print details
	Node<T1, T2>* ptr2 =find(root, key, stars); //use find function to find correct node
	if(ptr2){ //if ptr2 is found
		ptr2->print(stars); //print details
	}
    else{
		cout<<"NOT FOUND"<<endl; //print not found message s
	}
	
}
template <typename T1, typename T2> Node<T1,T2>* BST<T1,T2>::findMin(Node<T1,T2> *ptr){
	if(ptr==nullptr){
		return nullptr; //if Tree is empty
	} 
	else if(ptr->left){
		return findMin(ptr->left); 	//recursive approach to find the node with minimum key in the tree
	}		
	else{
		return ptr;	//if reached to the left most node
	}	
}
template <typename T1, typename T2> void BST<T1,T2>::remove(Node<T1,T2> *ptr,T1 key, T2 value){
	if (ptr == nullptr)				//Node Not Found		
	{
		cout<<"Key not found"<<endl;	//Reached to bottom of the tree but key not found
		return;
	}
	if(key<ptr->key) 						//if key is smaller than the key at root of the tree/subtree
		remove(ptr->left, key, value);		//then search in left subtree
	else if(key>ptr->key)
		remove(ptr->right, key, value);	//else search in right subtree
	else if (key ==ptr->key){ //if matching key is found
		for(int i =0; i<ptr->value.size(); i++){
			if(ptr->value.at(i)->getName()==value->getName()){ //find the correct hotel to delete
				ptr->value.erase(ptr->value.begin() + i);  //delete vector
				return;
			}
		}
	}
}
template <typename T1, typename T2> int BST<T1,T2>::balanceFactor(Node<T1,T2> *ptr)//calculate balance factor
{
    int h1 = height(ptr->left);//height of left child
    int h2 = height(ptr->right);//height of right child

    return (h1-h2);//subtract
}
template <typename T1, typename T2> void BST<T1,T2>::rotateRight(Node<T1,T2> *z)
{
    Node<T1, T2> *y = z->left; //save the left child of z and its right child
    Node<T1, T2> *t2 = y->right;
    
    if (z->parent != nullptr) { //update z's parent to point to y instead of z
        if (z == z->parent->left) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }
    } else {
        root = y; //if z is the root, update the root to be y
    }

	//update the pointers
    y->parent = z->parent;
    y->right = z;
    z->parent = y;
    z->left = t2;
    if (t2 != nullptr) { //update the parent pointer of t2 to be z (if t2 is not nullptr)
        t2->parent = z;
    }
} 
//==========================================
// Rotate a node to its left  
template <typename T1, typename T2> void BST<T1,T2>::rotateLeft(Node<T1,T2> *z) //rotate left function
{
    Node<T1, T2> *y = z->right; //save the right child of z and its left child
    Node<T1, T2> *t2 = y->left;
    
    if (z->parent != nullptr) {//update z's parent to point to y instead of z
        if (z == z->parent->left) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }
    } else {
        root = y;//if z is the root, update the root to be y
    }

    y->parent = z->parent;//update the parent pointer
    y->left = z; //update the left child of y to be z
    z->parent = y;
    z->right = t2; //update the right child of z to be t2
    if (t2 != nullptr) { //update the parent pointer of t2 to be z (if t2 is not nullptr)
        t2->parent = z;
    }
}
//============================================
// Rebalance a node (ptr)
template <typename T1, typename T2> void BST<T1,T2>::rebalance(Node<T1,T2> *ptr)
{	
	while(ptr != nullptr){
		int bfactor = balanceFactor(ptr);
		//already balanced
		if(bfactor >= -1 && bfactor <= 1){
			return;
		}
		else{
			if(bfactor < -1){
				if(balanceFactor(ptr->right) <= 0){//right right heavy 
					rotateLeft(ptr); //use rotateLeft function to balance
				}
				else{// case 3: right left heavy
					rotateRight(ptr->right);//use rotateRight function to balance
					rotateLeft(ptr);//use rotateLeft function to balance
				}
			}
			if(bfactor > 1){
		        if (balanceFactor(ptr->left) >= 0) { // case 2 : left left heavy 
		            rotateRight(ptr);//use rotateRight function to balance
		        } else { // case 4: left right heavy 
		            rotateLeft(ptr->left);//use rotateLeft function to balance
		            rotateRight(ptr);//use rotateRight function to balance
		        }			
			}
		}
		ptr = ptr->parent;	
	}	
}