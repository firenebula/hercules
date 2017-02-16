#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <vector>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>

#include "Room.hpp"
#include "Item.hpp"
#include "Parser.hpp"



using std::cin;
using std::cout;
using std::endl;


enum LABORS {NEMEAN, LERNA, CERYNEIA};

void testParseVal(string, Parser);
Room loadRoom(std::map<string, Item*>& itemMap, std::map<string, Item*>& rmItems, string roomFile);
void loadItems(std::map<string, Item*>& itemMap, string itemFile);
Item makeItem(string itemFile);
string move(Room current, string destination);

void addInventory(std::map<string, Item*>& inventory, std::map<string, Item*>& itemMap, string itemName);
void printInventory(std::map<string, Item*>& inventory);
void addRoomItems(std::map<string, Item*>& roomItems, std::map<string, Item*>& itemMap, string itemName);
bool removeRoomItems(std::map<string, Item*>& roomItems, string itemName);
void saveRoom(std::map<string, Item*>& roomItems, Room current);
void printRoomItem(std::map<string, Item*>& roomItems);

void checkForEvent(LABORS currentLabor, string currentRoom, string command, std::map<string, string>& eventActions);

void setLabor(LABORS &currentLabor, string newLabor);

void saveInventory(std::map<string, Item*>& inventory);
void loadInventory(std::map<string, Item*>& itemList, std::map<string, Item*>& inventory);
void saveLabor(LABORS currentLabor);
void scanDirectory(std::vector<string>& dir_contents, string dir_path);
int cleanInput(string& input, int& valid);
void saveGame(std::map<string, Item*>& roomItems, Room current, std::map<string, Item*>& inventory, LABORS currentLabor, string& saveAs);

int main()
{

	string command;
	string saveAs = "";
	string itemFile = "./items";
	std::map <string, Item*> itemList;
	std::map <string, Item*> inventory;
	std::map <string, Item*> roomItems;
	
	std::map <string, string> eventActions;

	Item testItem = makeItem(itemFile);

	LABORS currentLabor = NEMEAN;

	Parser hParser;
//	testParseVal("Default Values: ", hParser);
/*	
	std::cout << "\n****Testing Parser.  Type start to start game.****" << std::endl;
    while (hParser.getObject().compare("start") != 0) {
		cout << endl << "What do you want to do?  ";
		std::getline(cin, command);
		hParser.parse(command);
		testParseVal("Current Values:", hParser);


    }
*/	

	command = "";
	
	// load game items
	loadItems(itemList, itemFile);

	// start game with club in inventory
	addInventory(inventory, itemList, "club");

	// start game in throne room
	Room current = loadRoom(itemList, roomItems, "throne");

	while (hParser.getAction().compare("quit") != 0) {
		cout << endl << "What do you want to do?  ";
		std::getline(cin, command);
		hParser.parse(command);

		if (hParser.getAction().compare("look") == 0) {
			string lookItem = hParser.getObject();
			if (lookItem.compare("$none") == 0) 
				cout << current.look();
			else {
				bool foundItem = false;
				
				// check if "club" in inventory
				if(inventory.find(lookItem) != inventory.end()) {
					foundItem = true;
				}
				// if "club" not in inventory then check if in the room
				else if(roomItems.find(lookItem) != roomItems.end()) {
					// check if the item is unlocked
					//if (roomItems[lookItem]->isAvailable())
						foundItem = true;
				}
				if (foundItem)
					cout << itemList[lookItem]->look() << endl;
				else {
					command = "";
					cout << "You look at the "<< lookItem << " but realize that it's not really there. It was just a mirage!" << endl;
				}
			}
			
			printRoomItem(roomItems);
		}
		
		else if (hParser.getAction().compare("move") == 0) {
			string destination = hParser.getObject();
			destination = move(current, destination);
			if (destination.compare("null") == 0) {
				cout << "You cannot go there!" << endl;
				command = "";
			}
			else {
				// save room data
				saveRoom(roomItems, current);
				roomItems.clear();
				current = loadRoom(itemList, roomItems, destination);
			}
		}
		
		else if (command.find("inventory") != std::string::npos) {
			printInventory(inventory);
		}
		
		else if (command.compare("look club") == 0) {
			string lookItem = "club";
			bool foundItem = false;
			// check if "club" in inventory
			if(inventory.find(lookItem) != inventory.end()) {
					foundItem = true;
				}
			// if "club" not in inventory then check if in the room
			else if (!roomItems.empty()) {
				if(roomItems.find(lookItem) != roomItems.end()) {
					foundItem = true;
				}
			}
			if (foundItem)
				cout << itemList[lookItem]->look() << endl;
			else {
				command = "";
				cout << "You look at the club but realize that it's not really there. It was just a mirage!" << endl;
			}
		}
		
		else if (hParser.getAction().compare("drop") == 0) {
			// check if item is in inventory
			string dropObj = hParser.getObject();
			if(inventory.find(dropObj) != inventory.end()) {
				inventory[dropObj]->drop();
				addRoomItems(roomItems, itemList, dropObj);
				// remove item from inventory if quantity is 0
				if (inventory[dropObj]->getQuantity() == 0)
					inventory.erase(dropObj);
				cout << "You dropped a "<< dropObj << " on to the ground." << endl;
			}
			else {
				cout << "You dropped a " << dropObj << " but it vanishes before it hit the ground. " << endl;
			    cout << "You realized you were hallucinating it all this time" << endl;
				command = "";
			}
		}
		else if (hParser.getAction().compare("get") == 0) {
			string itemName = hParser.getObject();
		//else if (command.compare("take club") == 0) {
		//	string itemName = "club";
			if (removeRoomItems(roomItems, itemName)) {
				addInventory(inventory, itemList, itemName);
				cout << "You picked up the " << itemName << endl;
			}
			else {
				command = "";
				cout << "You can't pick that up!" << endl;
			}
		}
		/*
		else if (command.compare("drop lion pelt") == 0) {
			// check if item is in inventory
			if(inventory.find("lion pelt") != inventory.end())
			{
				inventory["lion pelt"]->drop();
				addRoomItems(roomItems, itemList, "lion pelt");

				if (inventory["lion pelt"]->getQuantity() == 0)
					inventory.erase("lion pelt");
				cout << "You dropped a lion pelt on to the ground." << endl;
			}
			else {
				cout << "You dropped a lion pelt but it vanishes before it hit the ground. " << endl;
			    cout << "You realized you were hallucinating it all this time" << endl;
			}
		}
		
		
		else if (command.compare("take lion pelt") == 0) {
			string itemName = "lion pelt";
			if (removeRoomItems(roomItems, itemName)) {
				addInventory(inventory, itemList, itemName);
				cout << "You picked up the " << itemName << endl;
			}
			else
				cout << "You can't pick that up!" << endl;
		}
		*/
/*
		else if (command.compare("look pokeball") == 0) {
			// check if item is in inventory
			if (testItem.getLocation().compare("inventory") == 0) {
				testItem.drop(current.getName());
				cout << "You dropped a " << testItem.getName() << " on to the ground." << endl;
			}
			else {
				cout << "You dropped a " << testItem.getName() << " but it vanishes before it hit the ground. " << endl;
			    cout << "You realized you were hallucinating it all this time" << endl;
			}
		}
*/
		else if (command.find("save") != std::string::npos)
			saveGame(roomItems, current, inventory, currentLabor, saveAs);
			
		else if (command.compare("quit") != 0)
			cout << "I don't understand that command!" << endl;

		
		checkForEvent(currentLabor, current.getName(), command, eventActions);
		
		if (!eventActions.empty()) {
			for(map<string, string>::iterator it = eventActions.begin(); it != eventActions.end(); ++it) {
				//cout << it->first << " : " << eventActions[it->first] << endl; 
				if ((it->first).compare("display") == 0) {
					cout << eventActions[it->first] << endl;
				}
				else if ((it->first).compare("change state") == 0) {
					setLabor(currentLabor, eventActions[it->first]);
				}
				else if ((it->first).compare("add exit south") == 0) {
					current.setExits(1, eventActions[it->first]);
				}
				else if ((it->first).compare("add item") == 0) {
					addRoomItems(roomItems, itemList, eventActions[it->first]);
				}
			}
			eventActions.clear();
			
		}
//		else 
//			cout << "No event triggered!" << endl;
		
		
		
	}

	saveRoom(roomItems, current);
    return 0;
}

Room loadRoom(std::map<string, Item*>& itemMap, std::map<string, Item*>& rmItems, string roomFile) {
	string data;
//	bool visited;
	string desc_long = "";
	string desc_short = "";
	string itemName;
	std::fstream save_room;
	std::fstream room_file;
	Room current(roomFile);

	// convert roomFile to lowercase
	for(unsigned int i = 0; i < roomFile.length(); i++) {
		roomFile[i] = tolower(roomFile[i]);
	}

	// create file path string
	string room_path = "./rooms/";   // original room directory
	room_path.append(roomFile);
	room_path.append(".room");

	string save_path = "./save/";	 // save room directory
	save_path.append(roomFile);
	save_path.append(".room");


	// convert room_path to c-string for fstream open() and open save file
	save_room.open(save_path.c_str(), std::ios::out | std::ios::in);

	// if room file is not in save directory, then copy original room file
	// to save directory and open it
	if (!save_room) {
//		cout << "Cannot find save room file, creating file : [" << save_path << "]" << endl;
		save_room.close();

		// convert room_path to c-string for fstream open() and open room file
		room_file.open(room_path.c_str(), std::ios::out | std::ios::in);

		std::ifstream src(room_path.c_str(), std::ios::binary);
		std::ofstream dest(save_path.c_str(), std::ios::binary);
		dest << src.rdbuf();
		src.close();
		dest.close();
		room_file.close();

		if (!(src && dest))
			cout << "Cannot create save room file" << endl;
/*		else {
			cout << "Save room file created" << endl;
			//save_room.open(save_path.c_str(), std::ios::out | std::ios::in );
		}
*/
	}

	room_path = "./save/";   // original room directory
	room_path.append(roomFile);
	room_path.append(".room");
	room_file.open(room_path.c_str(), std::ios::out | std::ios::in);

	if(room_file) {
		// get 1st line of text which is the room's name
		std::getline(room_file, data);
		string name = data;

		// get lines of text until visited section is reached
		while (data.compare("visited") != 0) {
			std::getline(room_file, data);
		}
		std::getline(room_file, data);

		if (data.compare("true") == 0)
				current.setVisited(true);
			else
				current.setVisited(false);


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
			if (data.compare("") != 0) {
				desc_long.append(data);
				desc_long.append("\n");
			}
			std::getline(room_file, data);
		}

		current.setDescLong(desc_long);

		std::getline(room_file, data);
		while(data.compare("items") != 0)
		{
			if (data.compare("") != 0) {
				desc_short.append(data);
				desc_short.append("\n");
			}
			std::getline(room_file, data);
		}

		current.setDescShort(desc_short);

		// all the lines of text in the "items" section and add them to
		// the roomItems map
		while (std::getline(room_file, data)) {
			if (data.compare("") != 0) {
				itemName = data;
				addRoomItems(rmItems, itemMap, itemName);
				std::getline(room_file, data);
				rmItems[itemName]->setQuantity(atoi(data.c_str()));
			}
			//cout << "data line from room file = [" << data << "]" << endl;
		}
    }
    else
       {
         cout << "could not find room file" << endl;
       }

	//current.printRoom();
	room_file.close();


	cout << endl << current.look();
	current.setVisited(true);

	return current;

}


void saveRoom(std::map<string, Item*>& roomItems, Room current) {
	std::ofstream save_room;
	string roomFile = current.getName();

	// convert roomFile to lowercase
	for(unsigned int i = 0; i < roomFile.length(); i++) {
		roomFile[i] = tolower(roomFile[i]);
	}

	//cout << "Saving room, [" << roomFile << "]" << endl;

	string save_path = "./save/";	 // save room directory
	save_path.append(roomFile);
	save_path.append(".room");

	// convert room_path to c-string for fstream open() and open save file
	save_room.open(save_path.c_str(), std::ios::out | std::ios::trunc);

	if(save_room) {
		// 1st line of text which is the room's name
		save_room << current.getName() << "\n\n";

		// visited section
		save_room << "visited\n";
		save_room << "true\n\n";

		// exits section
		save_room << "exits\n";
		for (int j = 0; j < NUM_OF_EXITS; j++) {
			save_room << current.getExit(j) << "\n";
		}
		save_room << "\n";

		// long description section
		save_room << "long description\n";

		// set visited to false so that look() returns long description
		current.setVisited(false);
		save_room << current.look();
		save_room << "\n";

		// short description section
		save_room << "short description\n";

		// set visited to true so that look() returns short description
		current.setVisited(true);
		save_room << current.look();
		save_room << "\n";

		// items section
		save_room << "items\n";
		for(map<string,Item*>::iterator it = roomItems.begin(); it != roomItems.end(); ++it) {
			save_room << it->first << "\n";
			save_room << roomItems[it->first]->getQuantity() << "\n";
		}

    }
    else
       {
         cout << "could not create save room file" << endl;
       }

	save_room.close();

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


Item makeItem(string itemFile) {
	Item testClub("club");
	testClub.setDesc("It's wrapped in barbed wire. I call her 'Lucille' and she is thirsty for blood.");
	testClub.setUse("You swing the club");
	testClub.setMovable(true);
	testClub.setQuantity(1);

	return testClub;

}


void loadItems(std::map<string, Item*>& itemMap, string itemFile) {
	string data = "";
	string name = "";
	string desc = "";
	string use = "";
	bool movable;
	int quantity;
	bool available;

	Item * tempItem;

	std::fstream item_file;

	// convert itemFile to lowercase
	for(unsigned int i = 0; i < itemFile.length(); i++) {
		itemFile[i] = tolower(itemFile[i]);
	}

	// convert itemFile to c-string for fstream open()
	item_file.open(itemFile.c_str(), std::ios::out | std::ios::in);

//	cout << "Item file opened = " << item_file << endl;

	if(item_file) {
		while(std::getline(item_file, data)) {
	//		data = "";
			name = "";
			desc = "";
			use = "";
			movable = true;

			while (data.compare("[name]") != 0) {
				std::getline(item_file, data);
			}

			// get 1st line of text which is the item's name
			std::getline(item_file, data);
			name = data;

			// get lines of text until description section is reached
			while (data.compare("[description]") != 0) {
				std::getline(item_file, data);
			}

			// get lines of text until use section is reached
			// and store those lines into variable, desc
			std::getline(item_file, data);
			while(data.compare("[use]") != 0)
			{
				desc.append(data);
				desc.append("\n");
				std::getline(item_file, data);
			}
			// remove last 2 newline char
			desc.erase(desc.length() - 2);

			// get lines of text until use section is reached
			while (data.compare("[use]") != 0) {
				std::getline(item_file, data);
			}

			// get lines of text until movable section is reached
			// and store those lines into variable, use
			std::getline(item_file, data);
			while(data.compare("[movable]") != 0)
			{
				use.append(data);
				use.append("\n");
				std::getline(item_file, data);
			}
			// remove last 2 newline char
			use.erase(use.length() - 2);

			// store next line after "movable" into variable, movable
			std::getline(item_file, data);

			if (data.compare("true") == 0)
				movable = true;
			else
				movable = false;

			// get line of text until quantity section is reached
			std::getline(item_file, data);
			while(data.compare("[quantity]") != 0)
			{
				std::getline(item_file, data);
			}

			// convert next line after "quantity" into an int and
			// store the value into the variable, quantity
			std::getline(item_file, data);
			quantity = atoi(data.c_str());

			// get line of text until available section is reached
			std::getline(item_file, data);
			while(data.compare("[available]") != 0)
			{
				std::getline(item_file, data);
			}

			// convert next line after "available" into a boolean
			// and store the value into the variable, available
			std::getline(item_file, data);
			if (data.compare("true") == 0)
				available = true;
			else
				available = false;

			 itemMap.insert(std::make_pair(name, new Item(name, desc, use, movable, quantity, available)));

		}

       }
     else
       {
         cout << "could not find item file" << endl;
       }

	item_file.close();

}

void addInventory(std::map<string, Item*>& inventory, std::map<string, Item*>& itemMap, string itemName) {

/*
	cout << "Old inventory Items:" << endl << "[";
	for(map<string,Item*>::iterator it = inventory.begin(); it != inventory.end(); ++it) {
		cout << it->first << "\n"; }
	cout << "]" << endl;
*/

	if (itemMap.find(itemName) == itemMap.end()) {
		cout << "Cannot add [" << itemName << "] because it does not exist!" << endl;
	}
	else if (inventory.find(itemName) == inventory.end()) {
		inventory.insert(std::make_pair(
		itemName,
		new Item(
			itemName,
			itemMap[itemName]->look(),
			itemMap[itemName]->use(),
			itemMap[itemName]->isMovable(),
			itemMap[itemName]->getQuantity(),
			itemMap[itemName]->isAvailable())));
	}
	else {
		inventory[itemName]->setQuantity(inventory[itemName]->getQuantity() + 1);
	}
/*
	cout << "New inventory Items:" << endl << "[";
	for(map<string,Item*>::iterator it = inventory.begin(); it != inventory.end(); ++it) {
		cout << it->first << "\n"; }
	cout << "]" << endl;
*/
}


void printInventory(std::map<string, Item*>& inventory) {
	cout << "Inventory:" << endl;
	for(map<string,Item*>::iterator it = inventory.begin(); it != inventory.end(); ++it) {
		cout << it->first << "\t";
		cout << inventory[it->first]->getQuantity() << "\n";
	}
}


void printRoomItem(std::map<string, Item*>& roomItems) {
	cout << "Room Inventory:" << endl;
	for(map<string,Item*>::iterator it = roomItems.begin(); it != roomItems.end(); ++it) {
		if (roomItems[it->first]->getQuantity() > 0) {
			cout << it->first << "\t";
			cout << roomItems[it->first]->getQuantity() << "\n";
		}
	}
}


void addRoomItems(std::map<string, Item*>& roomItems, std::map<string, Item*>& itemMap, string itemName) {
/*
	cout << "Old Room Items:" << endl << "[";
	for(map<string,Item*>::iterator it = roomItems.begin(); it != roomItems.end(); ++it) {
		cout << it->first << "\n"; }
	cout << "]" << endl;
*/

	if (itemMap.find(itemName) == itemMap.end()) {
		cout << "Cannot add [" << itemName << "] because it does not exist!" << endl;
	}
	else if (roomItems.find(itemName) == roomItems.end()) {
		roomItems.insert(std::make_pair(
		itemName,
		new Item(
			itemName,
			itemMap[itemName]->look(),
			itemMap[itemName]->use(),
			itemMap[itemName]->isMovable(),
			itemMap[itemName]->getQuantity(),
			itemMap[itemName]->isAvailable())));
	}
	else {
		roomItems[itemName]->setQuantity(roomItems[itemName]->getQuantity() + 1);
	}
/*
	cout << "New Room Items:" << endl << "[";
	for(map<string,Item*>::iterator it = roomItems.begin(); it != roomItems.end(); ++it) {
		cout << it->first << "\n"; }
	cout << "]" << endl;
*/
}


bool removeRoomItems(std::map<string, Item*>& roomItems, string itemName) {
	//check if is in the room
	if (!roomItems.empty()) {
		if(roomItems.find(itemName) != roomItems.end()) {
			//check if item is movable and available and in the room
			if (roomItems[itemName]->isMovable() &&  roomItems[itemName]->isAvailable() &&
				roomItems[itemName]->getQuantity() > 0) {
					roomItems[itemName]->drop();
					if (roomItems[itemName]->getQuantity() == 0)
						roomItems.erase(itemName);
			}
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

// This test function is just printing out the contents of the Parse obj variables
void testParseVal(string label, Parser p){

    cout << label << endl << "Action Value: " << p.getAction() << endl
    << "Object Value: " << p.getObject() << endl
    << "Indirect Value: " << p.getIndirect() << endl;

}


void checkForEvent(LABORS currentLabor, string currentRoom, string command, std::map<string, string>& eventActions) {
	if (currentLabor == NEMEAN && command.compare("kill lion") == 0 && currentRoom.compare("nemean") == 0) {
		eventActions.insert(std::make_pair("display", "You killed the lion! You hear a loin club cry out for its father \nwhile another lion with scars and a dark mane roars in approval!"));
		eventActions.insert(std::make_pair("add item", "lion pelt"));
	}
	
	
	
	if (currentLabor == NEMEAN && command.compare("drop lion pelt") == 0 && currentRoom.compare("throne") == 0) {
		eventActions.insert(std::make_pair("change state", "lerna"));
		eventActions.insert(std::make_pair("add exit south", "lerna"));
		eventActions.insert(std::make_pair("display", "The king ordered you to go kill the hydra of lerna!\n"));
	}

	
	
}

void setLabor(LABORS &currentLabor, string newLabor) {
	// enum LABORS {NEMEAN, LERNA, CERYNEIA};
	if (newLabor.compare("lerna") == 0)
		currentLabor = LERNA;
	else if (newLabor.compare("ceryneia") == 0)
		currentLabor = CERYNEIA;
	
}

void saveInventory(std::map<string, Item*>& inventory) {
	string save_path = "./save/inventory.inventory";
	std::ofstream dest(save_path.c_str(), std::ios::binary);
	//iterate through inventory. write item name to file (one name per line and one name per quantity owned)
	for(map<string,Item*>::iterator it = inventory.begin(); it != inventory.end(); ++it) {
		if (it == inventory.begin()) {
			dest << it->first;
		} else {
			dest << "\n" << it->first;
		}
		for(int i = 1; i < inventory[it->first]->getQuantity(); i++) {
			dest << "\n" << it->first;
		}
	}
	dest.close();
}

void loadInventory(std::map<string, Item*>& itemList, std::map<string, Item*>& inventory) {
	string item;
	std::fstream inventory_file;
	string inventory_path = "./save/inventory.inventory";
	inventory_file.open(inventory_path.c_str(), std::ios::out | std::ios::in);
	
	if (inventory_file) {
		//get each line of file which contains items to be added to inventory
		while (std::getline(inventory_file, item)) {
			addInventory(inventory, itemList, item);
		}
	} else {
		cout << "could not find/open inventory file.\n";
	}
	inventory_file.close();
}

void saveLabor(LABORS currentLabor) {
	string save_path = "./save/labor.labor";
	std::ofstream dest(save_path.c_str(), std::ios::binary);
	dest << currentLabor;
	dest.close();
}

void scanDirectory(std::vector<string>& dir_contents, string dir_path) {
	DIR *dir = NULL;
	struct dirent *drnt = NULL;
	
	dir_contents.clear();
	dir = opendir(dir_path.c_str());
	if (dir) {
		while(drnt = readdir(dir)) {
			dir_contents.push_back(drnt->d_name);
		}
		closedir(dir);
	} else {
		cout << "cannot open directory: " << dir_path << endl;
	}
}

int cleanInput(string& input, int& valid) {
	int i, j = 0, k = 0;
	int first_char = 1, modified = 1;
	string checks = "_ -";
	
	for (i = 0; i < input.length(); i++) {
		if (isalpha(input[i])) {
			input[j] = input[i];
			first_char = 0;
			valid = 0;
			k = j;
			j++;
		} else if (isdigit(input[i])) {
			input[j] = input[i];
			first_char = 0;
			valid = 0;
			k = j;
			j++;
		} else if (input[i] == checks[0]) {
			if (first_char == 0) {
				input[j] = input[i];
				j++;
			} else {
				modified = 0;
			}
		} else if (input[i] == checks[1]) {
			if (first_char == 0) {
				input[j] = checks[0];
				j++;
				modified = 0;
			} else {
				modified = 0;
			}
		} else if (input[i] == checks[2]) {
			if (first_char == 0) {
				input[j] = input[i];
				j++;
			} else {
				modified = 0;
			}
		}
	}
	input.resize(k + 1);
	cout << input << endl;
	return modified;
}

void saveGame(std::map<string, Item*>& roomItems, Room current, std::map<string, Item*>& inventory, LABORS currentLabor, string& saveAs) {
	int i, j, k;
	int save = 1, overwrite = 1, validInput = 1, modified = 1;
	string yes_no;
	
	//search through saved_games directory for names of saved games and store them in a vector
	std::vector<string> game_names;
	scanDirectory(game_names, "saved_games");
	
	//save current room, inventory, and labor to save folder before copying
	saveRoom(roomItems, current);
	saveInventory(inventory);
	saveLabor(currentLabor);
	
	if (saveAs != "") {
		cout << "You either loaded or previously saved the game titled: " << saveAs << ".\nWould you like to save with this same name and overwrite the game files?\n";
		std::getline(cin, yes_no);
		for(i = 0; i < yes_no.length(); i++) {
			yes_no[i] = tolower(yes_no[i]);
		}
		if ((yes_no == "y") || (yes_no == "yes")) {
			save = 0;
			overwrite = 0;
		}
	}

	while (save == 1) {
		save = 0;
		cout << "What name would you like to save the game under?: ";
		std::getline(cin, saveAs);
		modified = cleanInput(saveAs, validInput);
		while (validInput == 1) {
			cout << "Your saved game name was not valid. It must contain at least one alphanumeric character. Please enter a new name: ";
			std::getline(cin, saveAs);
			modified = cleanInput(saveAs, validInput);
		}
		if (modified == 0) {
			cout << "FYI... The name you input was revised to " << saveAs << ".\n";
		}
		for (j = 0; j < game_names.size(); j++) {
			if (saveAs == game_names.at(j)) {
				cout << saveAs << " already exists. Would you like to overwrite the game files?\n";
				std::getline(cin, yes_no);
				for(i = 0; i < yes_no.length(); i++) {
					yes_no[i] = tolower(yes_no[i]);
				}
				if ((yes_no == "y") || (yes_no == "yes")) {
					save = 0;
					overwrite = 0;
				} else {
					save = 1;
				}
			}
		}
	}
	
	if (overwrite == 0) { //need to delete current directory before saving
		string remove = "rm -r -f saved_games/";
		remove.append(saveAs);
		remove.append("/");
		system(remove.c_str());
	}
	string copy = "cp -r save/ saved_games/";
	copy.append(saveAs);
	copy.append("/");
	system(copy.c_str());
	cout << "Game saved successfully as " << saveAs << ".\n";
}