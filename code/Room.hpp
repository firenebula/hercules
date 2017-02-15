/****************************************************
 * Author: Phuc Vo
 * Date: 01/24/2017
 * Description:
 *    The Room class is used to create a Room object 
 * for the text adventure game.
 *    Room.hpp is the class specification file.
 *
 * **************************************************/


#ifndef ROOM_HPP
#define ROOM_HPP

#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int NUM_OF_EXITS = 6;

class Room
{
   private:
      string name;
      string desc_short;
      string desc_long;
      bool visited;
      std::string exits[NUM_OF_EXITS];  // index 0 = north, 1 = south, 2 = east, 3 = west, 4 = up, 5 = down

	  
	  
   public:
	Room(std::string name);
	void setDescLong(std::string desc_long);
	void setDescShort(std::string desc_short);
	void setVisited(bool visited);
	string getName();
    bool getVisited();
	void setExits(int exit_index, std::string exit_name);
    string getExit(int exit_index);
    void printRoom();
    string look();
};
#endif
