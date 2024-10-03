#include "hotel.hpp"
#include "bst.hpp"

Hotel::Hotel(string name,string city, string stars,string price,string country,string address){ //constructor for hotel object
    this->name = name; //set name attribute to name given in argument 
    this->city=city; //set city attribute to city given in argument 
    this->stars=stars;//set stars attribute to stars given in argument 
    this->price = price; //set price attribute to price given in argument 
    this->country = country;//set country attribute to country given in argument 
    this->address = address; //set address attribute to address given in argument 
}
string Hotel::getName(){ //getter function for name
    return name; 
}
string Hotel::getCity(){//getter function for city
    return city; 
}
string Hotel::getStars(){//getter function for stars
    return stars;
}
string Hotel::getPrice(){//getter function for price
    return price;
}
string Hotel::getCountry(){//getter function for country
    return country;
}
string Hotel::getAddress(){//getter function for address
    return address;
}

void Hotel::print(){ //print hotel details for hashtable 
    cout<<"Hotel Name: "<<name<<endl; 
    cout<<"City: "<< city<<endl; 
    cout<<"Stars: "<<stars<<endl; 
    cout<<"Price: "<<price<<endl; 
    cout<<"Country : "<<country<<endl; 
    cout<<"Address : "<<address<<endl; 
}
void Hotel::print2(){ //print hotel details for bst 
	cout<<std::left<<setw(50)<<name<<" "<<setw(10)<<city<<" "<<setw(5)<<stars<<" "<<setw(10)<<price<<" "<<setw(20)<<country<<" "<<address<<endl;
}