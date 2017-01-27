/****************************************************
 * Author: Phuc Vo
 * Date: 01/24/2017
 * Description:
 *    The Room class is used to create a Room object
 * for the text adventure game.
 *    Room.cpp is the class implementation file.
 *
 ****************************************************/


 #include "Room.hpp"
 
/****************************************************
 * Room::Room(string)
 * This constructor takes the a string as a parameter
 * and sets the member variable, name, to its value.
 ****************************************************/
 
Room::Room(std::string name) {
	this->name = name;
	this->visited = false;
}


/****************************************************
 * Room::setDescLong(string)
 * This function takes the a string as a parameter
 * and sets the member variable, desc_long, to its 
 * value.
****************************************************/
 
 void Room::setDescLong(std::string desc_long) {
	 this->desc_long = desc_long; 
 }
 
 
/****************************************************
 * Room::setDescShort(string)
 * This function takes the a string as a parameter
 * and sets the member variable, desc_long, to its 
 * value.
****************************************************/
 
 void Room::setDescShort(std::string desc_short) {
	 this->desc_short = desc_short; 
 }
 
/****************************************************
 * Room::setVisited()
 * This function sets the member variable, visited, to
 * true.
****************************************************/
 
 void Room::setVisited(bool visited) {
	 this->visited = visited; 
 }
 
/*****************************************************
 * bool Room::getVisited()
 * This function returns the member variable, visited.
******************************************************/
 
 bool Room::getVisited() {
	 return this->visited; 
 } 
 
 
 /****************************************************
 * Room::setExits(int exit_index, string exit_name)
 * This function sets the member string array, exits.
 * The first parameter is the index array and is
 * associated with the direction of the exit as so:
 *    0 = North
 *    1 = South
 *    2 = East
 *    3 = West
 * The string parameter sets element of the array 
 * which is the name of the room. The room's name is
 * also the name of the file containing the room's 
 * data.
****************************************************/
 
 void Room::setExits(int exit_index, std::string exit_name) {
	 this->exits[exit_index] = exit_name; 
 }

 
 /****************************************************
 * Room::getExits(int exit_index)
 * This function returns the name of the exit specified
 * by the parameter, exit_index.
 *
****************************************************/

 string Room::getExit(int exit_index) {
	 return this->exits[exit_index]; 
 } 
 

 
 /****************************************************
 * Room::printRoom()
 * This function prints the room's data.
****************************************************/
 
 void Room::printRoom() {
	 cout << "Name: " << this->name << endl;
	 cout << "Long Description:" << endl << this->desc_long << endl;
	 cout << "Short Description:" << endl << this->desc_short << endl;
	 for (int i = 0; i < NUM_OF_EXITS; i++) {
		 cout << "Exit " << i << ": " << this->exits[i] << endl;
		 
	 }
 }
 
 
  /****************************************************
 * Room::look()
 * This function prints the room's long description if
 * the variable, visited is false. If visited is true
 * then this function prints the short description.
****************************************************/
 
 void Room::look() {
	 if (!this->visited) {
		cout << endl << this->desc_long;
	 }
	 else {
		cout << endl << this->desc_short;	 
	 }
 }
 
 
 
 
 