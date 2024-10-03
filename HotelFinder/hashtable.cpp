#include "hashtable.hpp"

HashTable::HashTable(int capacity){
    this->capacity = capacity; //capacity is the desired capacity inserted by the user 
    this->size =  0; //size is initially set to 0
    this->collisions = 0; 
    this->buckets = new list<Entry>[capacity];
    //initialize list<Entry> *buckets and BST<string,Hotel*> cityRecords
}

unsigned long HashTable::hashCode(string Key)
{
   unsigned int hashCode =0; int index=0; int a = 3; int b = 4; //set a and b to be equal to 3 and 4,  index  and hashcode are set to 0
   while(a %capacity ==0){ //if a is a multiple of the capacity, randomize 
       a = rand() % (capacity-1) + 1;
   }
   while(b %capacity ==0){ //if b is a multiple of the capacity, randomize 
       b = rand() % (capacity-1) + 1;
   }
   for(int i= 0; i<Key.length();i++){ //iterate through word
       hashCode = (hashCode<<5) | (hashCode>>27);// perform a circular bit shift to the left by 5 bits and a  bitwise OR operation with a circular bit shift to the right by 27 bits 
       hashCode += (unsigned int) Key[i]; //hashcode is equal to the  ascii code of the current letter of “key” casted from int to unsigned int
   }
   index = abs(static_cast<int>(a*hashCode+b)) % capacity; //for N , make sure a and b are not multiples on N (the number mod n can not be 0 )
   return index; //return value of index     
}

void HashTable::insert(string key, Hotel* value){ //insert function  
    int index = hashCode(key); //find index by using hashcode function
    Entry e(key,value); //create entry object 
    for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it)//iterate through list in buckets with correct index
    {
        if (it->key == key) //if key matches, duplicate key
        {
            buckets[index].erase(it); //erase original 
            break;
        }
    }
    buckets[index].push_front(e); //add to list containing keys (hotel,city) and values (hotel object)
    ++size; //increase size
    cityRecords.insert(cityRecords.root,value->getCity(), value ); //insert to bst as well
}
Hotel* HashTable::find(string key){ //find function to find given hotel 
    int index = hashCode(key); int comparisons = 0; //initialize variables 
    list<Entry>& bucket = buckets[index]; //bucket is a list in the corresponding index of hashtable
    bool found =false; 

    for(const Entry& entry : bucket){ //iterate through the list "bucket"
        comparisons++; //increase comparisons 
        if (entry.key==key){ //matching key found
            cout<<"Record found after "<<comparisons<< " comparison(s)"<<endl; 
            entry.value->print(); //print hotel info
            found =true; 
        }
    }
    if(found==false){ //if no key was found
        cout<<"Record not found."<<endl;
    }
}
void HashTable::findAll(string city,string stars){ //find hotels in corresponding city/stars 
    cityRecords.findPrint(city,stars); //use find print method of bst class

}
void HashTable::erase(string key){ //function to delete hotel from hashtable and bst 
    int index = hashCode(key);  bool found; string keyForBST; //initialize variables 
    list<Entry>& bucket = buckets[index];  //individual list in index of hashtable 
    Hotel* val; //create hotel object
    for (auto it = bucket.begin(); it != bucket.end(); ++it) //iterate through list  (bucket)
    {
        if (it->key == key)//if correct key is found 
        {
            found = true; //set found to true 
            val = it->value; //value of hotel object is the value of the list (hotel corresponding to key)
            bucket.erase(it); //erase from list 
            break;
        }
    }
    if(found == true){ //if record was found
        cout<<"Record deleted. "<<endl;
        --size; 
    }
    else{ //if record was not found
        cout<<"Record not found. "<<endl;
    }
    cityRecords.remove(cityRecords.root,val->getCity(),val); //remove from bst as well
}
unsigned int HashTable::getSize(){ //getter function for size 
    return size; 
}
void HashTable::incrementSize(){ //update size of hashtable 
    ++size; 
}
unsigned int HashTable::getCollisions(){ //getter function for collisions 
    return collisions; 
}
unsigned int HashTable::getCapacity(){ //getter function for capcity 
    return capacity; 
}
void HashTable:: countCollisions(){ //count the number of collisions in the hash function
   this->collisions = 0; //set the collisions attribute of the hashtable class to 0 before calculating  
   
   for(int i = 0; i<capacity; i++){ 
       list<Entry> b = buckets[i]; // new list object is created and is assigned to the list at buckets[i] (i depending on the current iteration of the for loop) 
       int numNodes = b.size(); //numNodes is set to be the number of elements in the list
       if(numNodes>0){ //if the list contains at least one element
           this->collisions += numNodes -1; //collisions is now equal to the current value of collions increased by the number of nodes in the list subtracted by 1
       }
   }
}
void HashTable::dump(string path){
    ofstream file; //create ofstream object 
    file.open(path); //open file
    if (file.is_open()){
        for(int i =0; i<capacity; i++){ //iteratively add information of each hotel into the csv file 
            list<Entry> b = buckets[i];
            for(auto it = b.begin(); it != b.end(); ++it){
                file<<it->value->getName()<<","<<it->value->getCity()<<it->value->getStars()<<it->value->getPrice()<<it->value->getCountry()<<it->value->getAddress()<<endl;
            }
        }
    }
    file.close(); //close file
    cout<<size<<" records have been successfully exported to "<< path<<endl;
}
HashTable::~HashTable(){ //destructor function
    for (int i = 0; i < capacity; i++) {
        buckets[i].clear(); //clear function deletes all nodes in the linked list
    }
    //delete the array of linked lists
    delete[] buckets;
}