#include<iostream>
#include<exception>
#include<fstream>
#include<sstream>
#include<string>
#include<math.h>
#include<list>
#include<cstdio>
#include "hashtable.hpp"

using namespace std;
Hotel* stringtoHotel(string details) //takes in string and converts to hotel object
{
	try	
	{
		string name,city,country,address,stars,price; //declare variables
		stringstream sstr(details); //stringstream object
		//get details from string about hotel
		getline(sstr,name,',');
		getline(sstr,city,',');
		getline(sstr,stars,',');
		getline(sstr,price,',');
		getline(sstr,country,',');
		getline(sstr,address);

		Hotel *hotel = new Hotel(name,city,stars,price,country,address); //create hotel object
		return hotel; //return hotel object
	}
	catch(exception &ex) //in case the user inputs incorrect format
	{
		cout<<"Invalid Hotel details. Please use the following format"<<endl;
		cout<<"hotelName,cityName,stars,price,countryName,address"<<endl;
		return nullptr;
	}
}
//=========================================================
void listCommands() //list commands user can use 
{
	cout<<"-------------------------------------------------------------------------------------------------------"<<endl;
	cout<<"List of Commands:"<<endl;
	cout<<"insert <Hotel Details>          :Add a new record to the Database using the formart <HotelName,CityName,Stars,Price,CountryName,Address>"<<endl;
	cout<<"find <HotelName,CityName>       :Find a specific hotel in the given City."<<endl;
	cout<<"findAll <CityName>              :Find all hotels of the given City."<<endl;
	cout<<"findAll <CityName,Stars>        :Find all hotels of the given City which are have the stars given as argument."<<endl;
	cout<<"delete <HotelName,CityName>     :Delete a specific record."<<endl;
	cout<<"dump <file>                     :Dump the content of the entire structure(s) into the given file."<<endl;
	cout<<"exit                            :Quit the program."<<endl;
	cout<<"-------------------------------------------------------------------------------------------------------"<<endl;
}
//======================================================
int main(int argc, char** args)
{
	HashTable myHashTable(125003); //create hashtable object
	
	if(argc==3 and strcmp(args[1],"-f")==0)
	{
		ifstream fin; //ifstream object
		cout << "Attempting to open file: " << args[2] << endl;
		fin.open(args[2]);
		if(!fin){ //if file is not able to be opened
			cout<<"Can not open the file " << args[2]<<endl;
			exit(-1);
		}

		string line; //declare variable
		getline(fin,line);  //skip first line
		while(!fin.eof()) //parse csv file and add to hashtable and bst
		{
			getline(fin,line);
			Hotel *h= stringtoHotel(line);
			string key = h->getName()+","+h->getCity();
			myHashTable.insert(key, h); 
		}
		fin.close();
	}
	cout<<"-------------------------------------------------------------------------------------------------------"<<endl;
	cout<<"Hash Table size = "<<myHashTable.getSize()<<endl; //print size of hashtable
	myHashTable.countCollisions(); 
	cout<<"Total number of collisions = "<<myHashTable.getCollisions()<<endl; //print collisions
	cout<<"% of collisions = "<<float(myHashTable.getCollisions())/myHashTable.getSize()*100<<endl;	//calculate % of collisions
	
	listCommands(); //list available commands to user
	while(true)
	{
		string user_input, command, argument1, argument2;
		cout<<">";
		getline(cin,user_input);
		//parse user input 
		stringstream sstr(user_input);
		getline(sstr,command,' ');
		getline(sstr,argument1,',');
		getline(sstr,argument2);

		//cout<<argument1<<endl<<argument2<<endl;

		if(command == "find")  //if command was find
		{
			Hotel *result = myHashTable.find(argument1+","+argument2); //use find method from hashtable
			if(result!=nullptr)	//cout<<result->toString()<<endl;
								// result->print();
			fflush(stdin);
			cin.clear();
		}
		else if(command == "findAll")//if command was findALL
		{
			myHashTable.findAll(argument1,argument2);//use findAll method from hashtable
			fflush(stdin);
			cin.clear();
		}
		else if(command == "insert") //if command was insert
		{
			Hotel *hotel = stringtoHotel(argument1+","+argument2);
			if(hotel!=nullptr)	//if the input is in correct format
				myHashTable.insert(hotel->getName()+","+hotel->getCity(),hotel);//use insert method from hashtable
		}
		else if(command == "delete")	myHashTable.erase(argument1+","+argument2);//if command was delete, use erase method from hashtable
		else if(command == "help") 		listCommands();
		else if(command == "dump")	    myHashTable.dump(argument1);//if command was dump, use dump method from hashtable
		else if(command == "exit")	  	break;
		else cout<<"Invalid command !!!"<<endl;
	}
	return 0;
}