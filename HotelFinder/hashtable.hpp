
#pragma once 
#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
#include<list>
#include "hotel.hpp"
#include "bst.hpp"

using namespace std;
class Entry
{
	private:
		string key;			//hotel name
		Hotel *value;		//hotel object
	public:
		Entry(string key, Hotel *value)
		{
			this->key = key;
			this->value = value;
		}
		friend class HashTable;
};
//===============================================================
class HashTable
{
	private:
		list<Entry> *buckets;		    			// Array of lists of type Entries for Chaining
		BST<string,Hotel*> cityRecords;
		unsigned int size;					   		//Current Size of HashTable
		unsigned int capacity;				    	// Total Capacity of HashTable
		unsigned int collisions; 					// Total Number of Collisions
	public:
		HashTable(int capacity);		
		unsigned long hashCode(string key); //calculate hashcode and find index
		void insert(string key, Hotel* value); //insert method (inserts to bst and hashtable)
		Hotel* find(string key); //find method
		void findAll(string city,string stars); //findAll method to find hotels w corresponding city/stars
		void erase(string key);  //erase method
		unsigned int getSize(); //getter function for size
		unsigned int getCollisions();//getter function for collisions
		unsigned int getCapacity(); //getter function for capacity
		void incrementSize(); //update size method
		void dump(string path); //dump contents of structures to file given by "path"
		void countCollisions(); //find num of collisions
		~HashTable(); //destructor for hashtable
};
//=================================================================
// implement the methods list above in a file called hashtable.cpp
