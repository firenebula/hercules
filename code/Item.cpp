/****************************************************
 * Author: Phuc Vo
 * Date: 01/30/2017
 * Description:
 *    The Item class is used to create a Item object 
 * for the text adventure game.
 *    Item.hpp is the class specification file.
 *
 * **************************************************/

#include "Item.hpp"

/*
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::string;
*/

 
/****************************************************
 * Item::Item(string)
 * This constructor takes the a string as a parameter
 * and sets the member variable, name, to its value.
 * It also sets the quantity to a default value of 0.
 ****************************************************/
 
Item::Item(string name) {
	this->name = name;
	this->movable = true;
	this->quantity = 1;
	this->available = true;
}


/*******************************************************
 * Item::Item(string, string, string, bool, int, bool)
 * This constructor takes the a string as a parameter
 * and sets the member variable, name, to its value.
 * It also sets the quantity to a default value of 0.
 *******************************************************/

Item::Item(string name, string desc, string use, string talk, bool movable, int quantity, bool available) {
	this->name = name;
	this->description = desc;
	this->use_text = use;
	this->talk_text = talk;
	this->movable = movable;
	this->quantity = quantity;
	this->available = available;
}


/****************************************************
 * Item::setDesc(string description);
 * This function takes a string as a parameter and 
 * sets the member variable, description, to its 
 * value.
****************************************************/
 
 void Item::setDesc(string description) {
	 this->description = description; 
 }
 
 
/****************************************************
 * void Item::setUse(string use_text)
 * This function takes a string as a parameter and
 * sets the member variable, action_use, to its 
 * value.
****************************************************/
 
 void Item::setUse(std::string use_text) {
	 this->use_text = use_text; 
 }
 
 
/****************************************************
 * void Item::setTalk(string talk_text)
 * This function takes a string as a parameter and
 * sets the member variable, action_use, to its 
 * value.
****************************************************/
 
 void Item::setTalk(std::string talk_text) {
	 this->talk_text = talk_text; 
 }
  
 
/****************************************************
 * void Item::setMovable(bool movable)
 * This function takes a bool as a parameter and
 * sets the member variable, movable, to its 
 * value.
****************************************************/
 
 void Item::setMovable(bool movable) {
	 this->movable = movable;
 }
 
/*****************************************************
 * void Item::setQuantity(int quantity)
 * This function takes an int as a parameter and
 * sets the member variable, quantity, to its 
 * value.
******************************************************/
 
 void Item::setQuantity(int quantity) {
	 this->quantity = quantity;
 } 
 
/*****************************************************
 * void Item::setAvailable(bool available)
 * This function takes a bool as a parameter and
 * sets the member variable, available, to its 
 * value.
******************************************************/
 
 void Item::setAvailable(bool available) {
	 this->available = available;
 } 
 
 
/****************************************************
 * string Item::getName()
 * This function returns the string stored in the 
 * variable, name. 
****************************************************/

 string Item::getName() {
	 return this->name;
 } 

 
/****************************************************
 * bool Item::isMovable()
 * This function returns the bool stored in the 
 * variable, movable. 
****************************************************/

 bool Item::isMovable() {
	 return this->movable;
 }  

 
/****************************************************
 * bool Item::isAvailable()
 * This function returns the bool stored in the 
 * variable, available. 
****************************************************/

 bool Item::isAvailable() {
	 return this->available;
 }   

 /****************************************************
 * int Item::getQuantity()
 * This function returns the int stored in the 
 * variable, quantity. 
****************************************************/

int Item::getQuantity() {
	return this->quantity;
}
 
 
 /****************************************************
 * string Item::look()
 * This function returns the string stored in the 
 * variable, description. This function is used to
 * return what text to display when the player "look"
 * at the Item object.
****************************************************/

 string Item::look() {
	 return this->description;
 } 

 /****************************************************
 * string Item::use()
 * This function returns the string stored in the 
 * variable, use. This function is used to
 * return what text to display when the player "use"
 * the Item object.
****************************************************/

 string Item::use() {
	 return this->use_text;
 }
 
 /****************************************************
 * string Item::use()
 * This function returns the string stored in the 
 * variable, description. This function is used to
 * return what text to display when the player "use"
 * the Item object.
****************************************************/

 string Item::use(string target) {
	string useItem = this->use_text;
	useItem.append(" on ").append(target);
	return useItem;
 } 
 
 
  /****************************************************
 * string Item::talk()
 * This function returns the string stored in the 
 * variable, talk. This function is used to
 * return what text to display when the player "talk"
 * to the Item object.
****************************************************/

 string Item::talk() {
	return this->talk_text;
 } 
 
 
  /****************************************************
 * void Item::drop()
 * This function is used to decrement an Item's
 * quantity when the player drops the Item object.
****************************************************/

 void Item::drop() {
	 (this->quantity)--;
 } 
 
 
 
 void Item::printItem() {
	 
	 cout << "Name: [" << this->name << "]" << endl;
	 cout << "Description:" << endl << "[" << this->description << "]" << endl;
	 cout << "Use:" << endl << "[" << this->use_text << "]" << endl;
	 cout << "Quantity: " << this->quantity << endl;
	 cout << "Movable: " << this->movable << endl;
	 cout << "Available: " << this->available << endl;
	 
 }
 
 