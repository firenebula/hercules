
#include <iostream>
#include <fstream>
#include <cstring>
#include "Room.hpp"
#include "Parser.h"

//using namespace std;
using std::cin;
using std::cout;
using std::endl;


void testParseVal(string, Parser);
Room loadRoom(string roomFile);
string move(Room current, string destination);

int main()
{
		
	string command;

	Parser hParser;  // new Parser Object
    testParseVal("Default Values: ", hParser); // default values set to -1
    hParser.parse("any input"); // will parse input.  For now, just updating to dummy values
    testParseVal("After Parse: ", hParser);

	
	// start game in throne roome
	Room current = loadRoom("Throne");	
		
	while (command.compare("quit") != 0) {
		cout << "What do you want to do?  ";
		std::getline(cin, command);
		
		// convert command to lowercase
		for(unsigned int i = 0; i < command.length(); i++) {
			command[i] = tolower(command[i]);
		}
		
		//cout << command << endl;
		
		if (command.compare("look") == 0)
			current.look();
		else if (command.find("go") != std::string::npos) {
			string destination = command.substr(3);
			//cout << destination << endl;
			//cout << command << endl;
			destination = move(current, destination);
			if (destination.compare("null") == 0)
				cout << "You cannot go there!" << endl;
			else
				current = loadRoom(destination);
		}
		//else if (command.compare("north") == 0)
		//	cout << move(current, "north") << endl;
		
	}
	
    return 0;
}

Room loadRoom(string roomFile) {
	std::string data;
	std::string desc_long = "";
	std::string desc_short = "";
	std::fstream room_file;
	Room current(roomFile);
	
	// convert roomFile to lowercase
	for(unsigned int i = 0; i < roomFile.length(); i++) {
		roomFile[i] = tolower(roomFile[i]);
	}
	
	// create file path string 
	string room_path = "./rooms/";
	room_path.append(roomFile);
	room_path.append(".room");
	
	// convert room_path to c-string for fstream open()
	room_file.open(room_path.c_str(), std::ios::out | std::ios::in);
	
	if(room_file)
       {
		   // get 1st line of text which is the room's name
			std::getline(room_file, data);
//			name = data;
//			cout << "Room: " << name << endl;
			
			// get "visited" line
//			room_file >> data;		
//			cout << data << endl;
			
			// get lines of text until exits section is reached
			while (data.compare("exits") != 0) {
				std::getline(room_file, data);
			}

			// store next several lines of text into the exit array of Room object
			for (int i = 0; i < NUM_OF_EXITS; i++) {
				std::getline(room_file, data);
				current.setExits(i, data);
			}
			
			// get lines of text until long description section is reached
			while(data.compare("long description") != 0) {
				std::getline(room_file, data);
			}
			
			// get lines of text until short description section is reached
			// and store those lines into desc_long variable of the Room object
			std::getline(room_file, data);
			while(data.compare("short description") != 0)
			{
				desc_long.append(data);
				desc_long.append("\n");
				std::getline(room_file, data);
			}
			current.setDescLong(desc_long);
			
			
			std::getline(room_file, data);
			while(data.compare("items") != 0)
			{
				desc_short.append(data);
				desc_short.append("\n");
				std::getline(room_file, data);
			}
			
			current.setDescShort(desc_short);
       }
     else
       {
         cout << "could not find room file" << endl;
       } 

	//current.printRoom();
	room_file.close();
	
	
	current.look();
	current.setVisited(true);

	return current;

}


string move(Room current, string destination) {
	// convert destination to lowercase
	for(unsigned int i = 0; i < destination.length(); i++) {
		destination[i] = tolower(destination[i]);
	}
	
	if (destination.compare("north") == 0)
		return current.getExit(0);
	else if (destination.compare("south") == 0)
		return current.getExit(1);
	else if (destination.compare("east") == 0)
		return current.getExit(2);
	else if (destination.compare("west") == 0)
		return current.getExit(3);	
	else {
		// search through all the room's exits and see if destination
		// matches any of the exit's name
		for (int i = 0; i < NUM_OF_EXITS; i ++)  {
			if (destination.compare(current.getExit(i)) == 0)
				return current.getExit(i);
		}
			
	}
	
	return "null";
	
}

// This test function is just printing out the contents of the Parse obj variables
void testParseVal(string label, Parser p){

    cout << label << endl << "Action Value: " << p.getAction() << endl
    << "Object Value: " << p.getObject() << endl
    << "Indirect Value: " << p.getIndirect() << endl;



}