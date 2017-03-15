#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <map>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <cstdlib>

#include "Room.hpp"
#include "Item.hpp"
#include "Parser.hpp"


using std::cin;
using std::cout;
using std::endl;


enum LABORS {NEMEAN, LERNA, CERYNEIA, ERYMANTHIA};
enum EXISTANCE {OBJ_EXISTS, HOLDING_OBJ, IND_EXISTS, HOLDING_IND};

void testParseVal(string, Parser);
void checkForExit(Room R, Parser& P);
Room loadRoom(std::map<string, Item*>& itemMap, std::map<string, Item*>& rmItems, string roomFile, int newGame = 1);
void loadItems(std::map<string, Item*>& itemMap, string itemFile);
void loadGameData(std::map<string, string>& gameData, string dataFile);

string move(Room current, string destination);

void addInventory(std::map<string, Item*>& inventory, std::map<string, Item*>& itemMap, string itemName);
void printInventory(std::map<string, Item*>& inventory);
void addRoomItems(std::map<string, Item*>& roomItems, std::map<string, Item*>& itemMap, string itemName);
bool removeRoomItems(std::map<string, Item*>& roomItems, string itemName);
void saveRoom(std::map<string, Item*>& roomItems, Room current);
bool isItemPresent(string itemName, std::map<string, Item*>& inventory, std::map<string, Item*>& roomItems);
bool isItemPresent(string itemName, std::map<string, Item*>& inventory);

void printRoomItem(std::map<string, Item*>& roomItems);

bool checkForEvent(LABORS currentLabor, string currentRoom, Parser hParser, bool existsArr[4], std::map<string, string>& gameData, std::map<string, string>& eventActions, std::map<string, Item*>& roomItems, Room current, std::map<string, Item*>& itemList, std::map<string, Item*>& inventory, string command);

void setLabor(LABORS &currentLabor, string newLabor);

void saveInventory(std::map<string, Item*>& inventory);
void loadInventory(std::map<string, Item*>& itemList, std::map<string, Item*>& inventory);
void saveLabor(LABORS currentLabor);
void saveCurrentRoom(Room current);
void saveGameData(std::map<string, string>& gameData);
void scanDirectory(std::vector<string>& dir_contents, string dir_path);
int cleanInput(string& input, int& valid);
void saveGame(std::map<string, Item*>& roomItems, Room current, std::map<string, Item*>& inventory, LABORS currentLabor, string& saveAs, std::map<string, string>& gameData);
void loadGame(std::map<string, Item*>& itemMap, std::map<string, Item*>& rmItems, Room& current, LABORS& currentLabor, std::map<string, Item*>& itemList, std::map<string, Item*>& inventory, string& saveAs, std::map<string, string>& gameData);

void printGameData(std::map<string, string>& gameData);



int main()
{

	string command;
	string saveAs = "";
	string itemFile = "./items";
	string dataFile = "./gamedata";
	string confirmLoad = "";
	int i, all_digits = 0, valid = 1, selection;
	LABORS currentLabor;
	std::map <string, Item*> itemList;
	std::map <string, Item*> inventory;
	std::map <string, Item*> roomItems;
	std::map <string, string> gameData;
	std::map <string, string> eventActions;
	bool existsArr[4];

	// load game items
	loadItems(itemList, itemFile);

	//create a Room object
	Room current = loadRoom(itemList, roomItems, "throne", 0);

	Parser hParser;

	std::vector<string> save_files;
	scanDirectory(save_files, "save");
	if (save_files.size() > 0) {
		system("exec rm -r save/*");
	}

	cout << "\nWelcome Hercules! What would you like to do?\n\nPlease type the number corresponding to your choice:\n1.Start a new game\n2.Load a saved game\n\n";
	std::getline(cin, command);
	for (i = 0; i < command.length(); i++) {
		if (!isdigit(command[i])) {
			all_digits = 1;
		}
	}
	if (all_digits == 0) {
		selection = std::atoi(command.c_str());
		if ((selection <= 2) && (selection > 0)) {
			valid = 0;
		}
	}
	while (valid == 1) {
		all_digits = 0;
		cout << "Your input was invalid. Please type the number corresponding to what you want to do:\n1.Start a new game\n2.Load a saved game\n\n";
		std::getline(cin, command);
		for (i = 0; i < command.length(); i++) {
			if (!isdigit(command[i])) {
				all_digits = 1;
			}
		}
		if (all_digits == 0) {
			selection = std::atoi(command.c_str());
			if ((selection <= 2) && (selection > 0)) {
				valid = 0;
			}
		}
	}
	if (selection == 2) { //check for saved games. If any exist ask user which to load
		std::vector<string> saved_games;
		scanDirectory(saved_games, "saved_games");
		if (saved_games.size() > 0) {
			loadGame(itemList, roomItems, current, currentLabor, itemList, inventory, saveAs, gameData);
		} else {
			cout << "Sorry, but there are no saved games. You'll have to start your adventure from the beginning!\n";
			selection = 1;
		}
	}
	if (selection == 1) { //new game: set current labor, clear out save directory, start with club and bow, load throne room

		cout << "\nHercules, welcome to Mycenae. I am King Eurystheus and\nper the Oracle's decree you will complete the labors I\nassign to you as penance for your follies. To aid you\non your way I gift you an olive wood club and a bow\nwith quiver of arrows. These should serve you well as\nyour first task is to bring me the Nemean lion - dead or alive.\n";

		// load game data
		loadGameData(gameData, dataFile);

		// set initial labor
		currentLabor = NEMEAN;

		// start game with bow and club in inventory
		addInventory(inventory, itemList, "club");
		addInventory(inventory, itemList, "bow");
		//addInventory(inventory, itemList, "lion pelt");

		// start game in throne room
		current = loadRoom(itemList, roomItems, "throne");
	}

	while (hParser.getAction().compare("quit") != 0) {


   //     while(true){

		cout << endl << "What do you want to do? ---> ";
		std::getline(cin, command);
		hParser.parse(command);
	//	testParseVal("***", hParser);

    //    }



//	cout << "[" << hParser.getAction() << "] [" << hParser.getObject() << "] [" << hParser.getIndirect() << "]" << endl;


		string objItem = hParser.getObject();
		string indItem = hParser.getIndirect();
	// change such that isPresent checks if object and indirect object are present!!!
		existsArr[OBJ_EXISTS] = isItemPresent(objItem, inventory, roomItems);
		existsArr[HOLDING_OBJ] = isItemPresent(objItem, inventory);
		existsArr[IND_EXISTS] = isItemPresent(indItem, inventory, roomItems);
        existsArr[HOLDING_IND] = isItemPresent(indItem, inventory);

		if (!checkForEvent(currentLabor, current.getName(), hParser, existsArr, gameData, eventActions, roomItems, current, itemList, inventory, command)) {

           checkForExit(current, hParser);


			if (hParser.getAction().compare("look") == 0) {
				string lookItem = hParser.getObject();
				if (lookItem.compare("$none") == 0) {
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
					cout << current.longLook();
					current.printExits();
					// for debugging
					printRoomItem(roomItems);
				}
				else {
					if (isItemPresent(lookItem, inventory, roomItems)) {
						cout << itemList[lookItem]->look() << endl;
						//itemList[lookItem]->printItem();
					}
					else {
						command = "";
						cout << "You look at the "<< lookItem << " but realize that it's not really there. It was just a mirage!" << endl;
					}
				}
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

			else if (hParser.getAction().compare("inventory") == 0) {
				printInventory(inventory);
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
				if (removeRoomItems(roomItems, itemName)) {
					addInventory(inventory, itemList, itemName);
					cout << "You picked up the " << itemName << endl;
				}
				else {
					command = "";
					cout << "You can't pick that up!" << endl;
				}
			}

			else if (hParser.getAction().compare("talk") == 0) {
				string itemName = hParser.getObject();
				if (isItemPresent(itemName, inventory, roomItems)) {
					cout << itemList[itemName]->talk() << endl;
				}
				else {
					command = "";
					cout << "What are you talking to?!" << endl;
				}
			}

			else if (hParser.getAction().compare("use") == 0) {
				string itemName = hParser.getObject();
//				if (isItemPresent(itemName, inventory)) {
				if (isItemPresent(itemName, inventory, roomItems)) {
					cout << itemList[itemName]->use() << endl;
				}
				else {
					command = "";
					cout << "You can't use that?!" << endl;
				}
			}

			else if (hParser.getAction().compare("save") == 0) {
				saveGame(roomItems, current, inventory, currentLabor, saveAs, gameData);
			}
			
			else if (hParser.getAction().compare("load") == 0) {
				cout << "Loading a game will cause you to lose your current progress. Are you sure you want to load a game?\n";
				std::getline(cin, confirmLoad);
				for(i = 0; i < confirmLoad.length(); i++) {
					confirmLoad[i] = tolower(confirmLoad[i]);
				}
				if ((confirmLoad == "y") || (confirmLoad == "yes")) {
					loadGame(itemList, roomItems, current, currentLabor, itemList, inventory, saveAs, gameData);
				} else {
					cout << "Did not load a game.";
				}
			}


			else if (hParser.getAction().compare("help") == 0) {
				cout << "HELP TEXT HERE!" << endl;
			}


			//else if (command.compare("quit") != 0 &&
			else if (hParser.getAction().compare("quit") != 0 &&
					!checkForEvent(currentLabor, current.getName(), hParser, existsArr, gameData, eventActions, roomItems, current, itemList, inventory, command)) {
				cout << "I don't understand that command!" << endl;
			}
			checkForEvent(currentLabor, current.getName(), hParser, existsArr, gameData, eventActions, roomItems, current, itemList, inventory, command);
		}

		if (!eventActions.empty()) {
			for(map<string, string>::iterator it = eventActions.begin(); it != eventActions.end(); ++it) {
				//cout << it->first << " : " << eventActions[it->first] << endl;
				if ((it->first).compare("display") == 0) {
					if (eventActions[it->first].compare("") != 0)
						cout << eventActions[it->first] << endl;
				}
				else if ((it->first).compare("change state") == 0) {
					setLabor(currentLabor, eventActions[it->first]);
				}
				else if ((it->first).compare("add exit") == 0) {
					string newExit = eventActions[it->first];
				//cout << "Add exit [" << newExit << "]" << endl;
					if (newExit.compare("north") == 0){
						current.setExits(0, eventActions[newExit]);
						current.setDisplay_Exits(0, eventActions[newExit]);
					}
					else if (newExit.compare("south") == 0){
						current.setExits(1, eventActions[newExit]);
						current.setDisplay_Exits(1, eventActions[newExit]);
					}
					else if (newExit.compare("east") == 0){
						current.setExits(2, eventActions[newExit]);
						current.setDisplay_Exits(2, eventActions[newExit]);
					}
					else if (newExit.compare("west") == 0){
						current.setExits(3, eventActions[newExit]);
						current.setDisplay_Exits(3, eventActions[newExit]);
					}
					else if (newExit.compare("up") == 0){
						current.setExits(4, eventActions[newExit]);
						current.setDisplay_Exits(4, eventActions[newExit]);
					}
					else if (newExit.compare("down") == 0){
						current.setExits(5, eventActions[newExit]);
						current.setDisplay_Exits(5, eventActions[newExit]);
				}

				}

				else if ((it->first).compare("remove exit") == 0) {
					string rmExit = eventActions[it->first];
					int exitIndex = -1;
					if (rmExit.compare("north") == 0)
						exitIndex = 0;
					else if (rmExit.compare("south") == 0)
						exitIndex = 1;
					else if (rmExit.compare("east") == 0)
						exitIndex = 2;
					else if (rmExit.compare("west") == 0)
						exitIndex = 3;
					else if (rmExit.compare("up") == 0)
						exitIndex = 4;
					else if (rmExit.compare("down") == 0)
						exitIndex = 5;

					if (exitIndex >= 0)
						current.setExits(exitIndex, "null");
						current.setDisplay_Exits(exitIndex, "null");
				}
				else if ((it->first).compare("remove room exit") == 0) {
					// eventActions[it->first] is the name of the non-current room
					// and the specific exit to be removed is stored in eventActions[eventActions[it->first]]
					string rmExit = eventActions[it->first];
					int exitIndex = -1;
					std::map <string, Item*> tempList;
					Room temp = loadRoom(itemList, tempList, rmExit, 0);
					rmExit = eventActions[rmExit];

					if (rmExit.compare("north") == 0)
						exitIndex = 0;
					else if (rmExit.compare("south") == 0)
						exitIndex = 1;
					else if (rmExit.compare("east") == 0)
						exitIndex = 2;
					else if (rmExit.compare("west") == 0)
						exitIndex = 3;
					else if (rmExit.compare("up") == 0)
						exitIndex = 4;
					else if (rmExit.compare("down") == 0)
						exitIndex = 5;

					if (exitIndex >= 0)
						temp.setExits(exitIndex, "null");
						temp.setDisplay_Exits(exitIndex, "null");

					//cout << "removed " << temp.getName() << "'s " << rmExit <<  " exit!" << endl;
					saveRoom(tempList, temp);
				}


				else if ((it->first).compare("add item") == 0) {
					addRoomItems(roomItems, itemList, eventActions[it->first]);
				}
				else if ((it->first).compare("remove item") == 0) {
					// make item movable so it can be removed
					roomItems[eventActions[it->first]]->setMovable(true);
					removeRoomItems(roomItems, eventActions[it->first]);
				}

				// drop item from inventory
				else if ((it->first).compare("drop item") == 0) {
					inventory[eventActions[it->first]]->drop();
					addRoomItems(roomItems, itemList, eventActions[it->first]);
					// remove item from inventory if quantity is 0
					if (inventory[eventActions[it->first]]->getQuantity() == 0)
						inventory.erase(eventActions[it->first]);
				}

				// consumes an item from inventory
				else if ((it->first).compare("consume item") == 0) {
					inventory[eventActions[it->first]]->drop();
					// remove item from inventory if quantity is 0
					if (inventory[eventActions[it->first]]->getQuantity() == 0)
						inventory.erase(eventActions[it->first]);
				}

				else if ((it->first).compare("get item") == 0) {
					addInventory(inventory, itemList, eventActions[it->first]);
				}

				else if ((it->first).compare("change short") == 0) {
					current.setDescShort(eventActions[it->first]);
				}

				else if ((it->first).compare("change long") == 0) {
					current.setDescLong(eventActions[it->first]);
				}
				else if ((it->first).compare("change talk") == 0) {
					string modifiedItem = eventActions[it->first];
					itemList[modifiedItem]->setTalk(eventActions[modifiedItem]);
				}

                else if ((it->first).compare("move") == 0) {
                string destination = eventActions[it->first];
                saveRoom(roomItems, current);
                roomItems.clear();
                current = loadRoom(itemList, roomItems, destination);
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

Room loadRoom(std::map<string, Item*>& itemMap, std::map<string, Item*>& rmItems, string roomFile, int newGame) {
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


		while (data.compare("dexits") != 0) {
			std::getline(room_file, data);
		}

		// store next several lines of text into the exit array of Room object
		for (int i = 0; i < NUM_OF_EXITS; i++) {
			std::getline(room_file, data);
			current.setDisplay_Exits(i, data);
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
		rmItems.clear();
		while (std::getline(room_file, data)) {
			if (data.compare("") != 0) {
				itemName = data;
				addRoomItems(rmItems, itemMap, itemName);
				std::getline(room_file, data);
		// debugging
				if (atoi(data.c_str()) == 0)
					cout << "\n\nMISSING QUANTITY FOR " << itemName << " IN " << roomFile << " ROOM FILE!" << endl;

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

	if (newGame == 1) {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << endl << current.look();
		current.printExits();
		printRoomItem(rmItems);

	}
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
		if (current.getVisited()) {
			save_room << "true\n\n";
		} else { //not visited
			save_room << "false\n\n";
		}

		// exits section
		save_room << "exits\n";
		for (int j = 0; j < NUM_OF_EXITS; j++) {
			save_room << current.getExit(j) << "\n";
		}
		save_room << "\n";

		//display exits section
		save_room << "dexits\n";
		for (int j = 0; j < NUM_OF_EXITS; j++) {
			save_room << current.getDisplay_Exit(j) << "\n";
		}
		save_room << "\n";

		// long description section
		save_room << "long description\n";

		// set visited to false so that look() returns long description
		save_room << current.longLook();
		save_room << "\n";

		// short description section
		save_room << "short description\n";

		// set visited to true so that look() returns short description
		save_room << current.shortLook();
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
	else if (destination.compare("up") == 0)
		return current.getExit(4);
	else if (destination.compare("down") == 0)
		return current.getExit(5);
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


void loadItems(std::map<string, Item*>& itemMap, string itemFile) {
	string data = "";
	string name = "";
	string desc = "";
	string use = "";
	string talk = "";
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
			talk = "";
			movable = true;

			while (data.compare("[name]") != 0) {
				std::getline(item_file, data);
			}

			// get 1st line of text which is the item's name
			std::getline(item_file, data);
			name = data;
//cout << "Name: " << name << endl;
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
//cout << "Desc: " << desc << endl;
			// get lines of text until use section is reached
			while (data.compare("[use]") != 0) {
				std::getline(item_file, data);
			}

			// get lines of text until talk section is reached
			// and store those lines into variable, use
			std::getline(item_file, data);
			while(data.compare("[talk]") != 0)
			{
				use.append(data);
				use.append("\n");
				std::getline(item_file, data);
			}
			// remove last 2 newline char
			use.erase(use.length() - 2);
//cout << "use: " << use << endl;
			// get lines of text until movable section is reached
			// and store those lines into variable, talk
			std::getline(item_file, data);
			while(data.compare("[movable]") != 0)
			{
				talk.append(data);
				talk.append("\n");
				std::getline(item_file, data);
			}
			// remove last 2 newline char
			talk.erase(talk.length() - 2);
//cout << "talk: " << talk << endl;
			// store next line after "movable" into variable, movable
			std::getline(item_file, data);

			if (data.compare("true") == 0)
				movable = true;
			else
				movable = false;
//cout << "movable: " << movable << endl;
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
//cout << "quantity: " << quantity << endl;
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
//cout << "available: " << available << endl;
			 itemMap.insert(std::make_pair(name, new Item(name, desc, use, talk, movable, quantity, available)));

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
			itemMap[itemName]->talk(),
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
	//cout << "Room Inventory:" << endl;
	for(map<string,Item*>::iterator it = roomItems.begin(); it != roomItems.end(); ++it) {
		//cout << "[" << it->first << "]" << "\t";

		if (roomItems[it->first]->getQuantity() == 1)
            cout << "*** " << it->first << " is here.\n";
        else
            cout << "There are " << roomItems[it->first]->getQuantity() << " " << it->first << " here.\n";

	//	cout << it->first << "\t";
	//	cout << roomItems[it->first]->getQuantity() << "\n";
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
			itemMap[itemName]->talk(),
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
		cout << it->first << "\t" << roomItems[it->first]->getQuantity() << "\n"; }

	cout << "]" << endl;
*/
}


bool removeRoomItems(std::map<string, Item*>& roomItems, string itemName) {
	//check if is in the room
	if (!roomItems.empty()) {
		if(roomItems.find(itemName) != roomItems.end()) {
			// check if item is movable and has a quantity > 0
			if (roomItems[itemName]->isMovable() && roomItems[itemName]->getQuantity() > 0) {
				roomItems[itemName]->drop();
				if (roomItems[itemName]->getQuantity() <= 0)
					roomItems.erase(itemName);
				return true;
			}
		}
	}

	return false;
}

// This test function is just printing out the contents of the Parse obj variables
void testParseVal(string label, Parser p){

    cout << label << endl << "Action Value: " << p.getAction() << endl
    << "Object Value: " << p.getObject() << endl
    << "Indirect Value: " << p.getIndirect() << endl;

}


bool checkForEvent(LABORS currentLabor, string currentRoom, Parser hParser, bool existsArr[4], std::map<string, string>& gameData, std::map<string, string>& eventActions, std::map<string, Item*>& roomItems, Room current, std::map<string, Item*>& itemList, std::map<string, Item*>& inventory, string command) {

    //existsArr positions OBJ_EXISTS, HOLDING_OBJ, IND_EXISTS, HOLDING_IND

	if (currentRoom.compare("woods3") == 0 && ((hParser.getAction().compare("move") == 0 && (hParser.getObject().compare("game trail") == 0 || hParser.getObject().compare("trail") == 0)) || (command.compare("game trail") == 0 || command.compare("trail") == 0))) {
		eventActions.insert(std::make_pair("move", "woods4"));
		if (gameData["hindLocation"].compare("woods4") == 0) {
			eventActions.insert(std::make_pair("display", "\nWhat's that? In the distance you see the glint of sunlight on gold.\nThe Hind heard your approach and scampered away before you\ncould get in range of a shot."));
			gameData["hindLocation"] = "woods5";
		}
		return true;
	}

	else if (currentRoom.compare("woods4") == 0 && ((hParser.getAction().compare("move") == 0 && (hParser.getObject().compare("game trail") == 0 || hParser.getObject().compare("trail") == 0)) || (command.compare("game trail") == 0 || command.compare("trail") == 0))) {
		eventActions.insert(std::make_pair("move", "woods3"));
		if (gameData["hindLocation"].compare("woods3") == 0) {
			if (gameData["trapSet"].compare("false") == 0) {
				eventActions.insert(std::make_pair("display", "\nWhat's that? In the distance you see the glint of sunlight on gold.\nThe Hind heard your approach and scampered away before you\ncould get in range of a shot."));
				gameData["hindLocation"] = "woods4";
			} else { //gameData["trapSet"] == "true"
				gameData["hindLocation"] = "caught";
				eventActions.insert(std::make_pair("display", "\nAgain the Hind hears you approach and flees. However, unfortunately\nfor the Hind it scampers towards the narrow path where your trap is\nset. You hear a commotion amongst the brush followed by a shrill yell\nfrom the Hind and are relieved\nto see the Hind dangling from the air with a hind\nleg caught in the snare. You rush over to the animal and quickly use the\nremaining rope to hog tie it before throwing the captured creature over your shoulder\nto haul back to Mycenae and King Eurystheus."));
				eventActions.insert(std::make_pair("get item", "hind"));
			}
		}
		return true;
	}

	else if (currentLabor == NEMEAN) {
		if (hParser.getAction().compare("move") == 0 && hParser.getObject().compare("shadow") == 0 && existsArr[OBJ_EXISTS] && currentRoom.compare("cave") == 0) {
			eventActions.insert(std::make_pair("display", "You stop and stare as the shadow starts changing.\nThe woman's hair suddenly starts growing and her body begins to enlarge.\nThe very large shadow lets out a large roar and rushes at you!"));
			eventActions.insert(std::make_pair("add item", "lion"));
			eventActions.insert(std::make_pair("remove item", "shadow"));
            eventActions.insert(std::make_pair("change long", "This is the den of the Nemean lion. It stinks of death and lion breath.\n"));
			return true;
		}

		else if ((hParser.getAction().compare("move") == 0 || hParser.getAction().compare("push") == 0)&& hParser.getObject().compare("boulder") == 0 && existsArr[OBJ_EXISTS] && currentRoom.compare("trail") == 0) {
			if (gameData["caveBlocked"].compare("false") == 0) {
				gameData["caveBlocked"] = "true";
				eventActions.insert(std::make_pair("display", "You pushed on the boulder and it starts to tip over. You jump out of the way as the boulder falls over and completely blocks the cave entrance."));

				// remove exit from lion cave
				//Room temp = loadRoom(itemList, roomItems, "nemean", 0);


				eventActions.insert(std::make_pair("remove exit", "west"));

				// remove exit from lion cave
				eventActions.insert(std::make_pair("remove room exit", "cave"));
				eventActions.insert(std::make_pair("cave", "east"));


				eventActions.insert(std::make_pair("change long", "The trail is covered by broken branches and animal tracks from a very large animal. A large boulder is completely blocking the cave entrance. The sounds of a woman crying emanate from inside the cave.\n"));
				eventActions.insert(std::make_pair("change short", "A trail leading to a cave. Go up the trail to head back up the canyon.\n"));
				eventActions.insert(std::make_pair("change look", "boulder"));
				eventActions.insert(std::make_pair("boulder", "You tried to push the boulder but it will not budge."));
				//block exit in cave room as well. load rooms without text output
				current = loadRoom(itemList, roomItems, "cave", 0);
				current.setExits(3, "null");
				saveRoom(roomItems, current);
				//go back to trail room so eventActions are performed on correct room
				current = loadRoom(itemList, roomItems, currentRoom, 0);
			}
			else {
				eventActions.insert(std::make_pair("display", "The boulder is fully settled in front of the cave entrance."));
			}
			return true;
		}
		// if the lion escaped and the hero goes another location, set flag, lionEscaped to signify the lion will return
		else if (hParser.getAction().compare("move") == 0 && currentRoom.compare("cave") != 0 && gameData["lionEscaped"].compare("true") == 0) {
			gameData["lionEscaped"] = "lionReturns";
			return false;
		}
		// if the flag, lionEscaped is lionReturns, and the hero goes back to the cave then add the lion
		else if (hParser.getAction().compare("move") == 0 && currentRoom.compare("cave") == 0 && gameData["lionEscaped"].compare("lionReturns") == 0) {
			gameData["lionEscaped"] = "false";
			eventActions.insert(std::make_pair("add item", "lion"));
			return true;
		}


		else if (hParser.getAction().compare("use") == 0 && hParser.getObject().compare("club") == 0 && existsArr[HOLDING_OBJ] && hParser.getIndirect().compare("lion") == 0 && currentRoom.compare("cave") == 0) {
			eventActions.insert(std::make_pair("display", "You swing the club at the lion but to your surprise the lion shrugs off the blow and rushes at you again."));
			return true;
		}

        else if (hParser.getAction().compare("attack") == 0 && hParser.getObject().compare("lion") == 0 && existsArr[OBJ_EXISTS] && hParser.getIndirect().compare("club") == 0 && existsArr[HOLDING_IND] && currentRoom.compare("cave") == 0) {
			eventActions.insert(std::make_pair("display", "You swing the club at the lion but to your surprise the lion shrugs off the blow and rushes at you again."));
			return true;
		}
		else if (hParser.getAction().compare("attack") == 0 && hParser.getObject().compare("lion") == 0 && existsArr[OBJ_EXISTS]
				&& hParser.getIndirect().compare("$none") == 0 && currentRoom.compare("cave") == 0) {
			if (atoi(gameData["lionAttacks"].c_str()) < 2) {
				eventActions.insert(std::make_pair("display", "You quickly duck under the outstretched claws of the lion and wrap your arms around the lion's throat. You began squeezing. The lion starts to thrash around the cave slamming you into the cave walls. You eventually lose your grip and let go of the lion. It is now breathing harder and moving slower but it does not let up and lunges at you again."));
				int lionAttacks = atoi(gameData["lionAttacks"].c_str()) + 1;
				std::stringstream ss;
				ss << lionAttacks;
				ss >> gameData["lionAttacks"];
			}
			else {
				if (gameData["caveBlocked"].compare("false") == 0){
					eventActions.insert(std::make_pair("display", "You once again wrestle with the lion grabbing the lion's throat.  The lion breaks free once more but this time it has enough of fighting with you. The lion starts retreating and runs toward the exit at the back of the cave. You chase after it but it is too late and the beast has already put too much distance between you. It doesn't look like it will be back here for a few days."));
					eventActions.insert(std::make_pair("remove item", "lion"));
					gameData["lionEscaped"] = "true";
					gameData["lionAttacks"] = "0";
				}
				else {
					eventActions.insert(std::make_pair("display", "You once again wrestle with the lion grabbing the lion's throat.  The lion breaks free once more but this time it has enough of fighting with you. It starts retreating and runs toward the exit at the back of the cave. However, the exit has been blocked. You seize the moment of confusion to jump on the lion once again and begin to squeeze its throat. It is too weak to fight back and finally succumbs to your death grip. The lion's body finally drops to the ground at your feet. You killed the lion!"));
					eventActions.insert(std::make_pair("remove item", "lion"));
					eventActions.insert(std::make_pair("add item", "lion pelt"));
				}
			}
			return true;
		}

		else if (hParser.getAction().compare("shoot") == 0 && hParser.getObject().compare("lion") == 0 && existsArr[OBJ_EXISTS] && hParser.getIndirect().compare("bow") == 0 && existsArr[HOLDING_IND] && currentRoom.compare("cave") == 0) {
			if ((rand() % 10) > 6) {
				eventActions.insert(std::make_pair("display", "SWHUP! The arrow shoots straight through the lion's mouth and into the back of its head. It lets out a welp before crashing into the ground. The lion's body spasms for a second or two before finally coming to a complete stop. You killed the lion! You hear a lion cub cry out for its father while another lion with scars and a dark mane roars in approval!"));
				eventActions.insert(std::make_pair("remove item", "lion"));
				eventActions.insert(std::make_pair("add item", "lion pelt"));
			}
			else {
				eventActions.insert(std::make_pair("display", "To your amazement your arrows bounce off the body of the lion. You barely dodge the lion's attack but the lion has already recovered and is coming at you again."));
			}

			return true;
		}

        else if (hParser.getAction().compare("shoot") == 0 && hParser.getObject().compare("bow") == 0 && existsArr[HOLDING_OBJ] && hParser.getIndirect().compare("lion") == 0 && existsArr[IND_EXISTS] && currentRoom.compare("cave") == 0) {
			if ((rand() % 10) > 6) {
				eventActions.insert(std::make_pair("display", "SWHUP! The arrow shoots straight through the lion's mouth and into the back of its head. It lets out a welp before crashing into the ground. The lion's body spasms for a second or two before finally coming to a complete stop. You killed the lion! You hear a lion cub cry out for its father while another lion with scars and a dark mane roars in approval!"));
				eventActions.insert(std::make_pair("remove item", "lion"));
				eventActions.insert(std::make_pair("add item", "lion pelt"));
			}
			else {
				eventActions.insert(std::make_pair("display", "To your amazement your arrows bounce off the body of the lion. You barely dodge the lion's attack but the lion has already recovered and is coming at you again."));
			}

			return true;
		}


		else if (hParser.getAction().compare("use") == 0 && hParser.getObject().compare("hyperbeam") == 0 && hParser.getIndirect().compare("lion")
				 && currentRoom.compare("cave") == 0) {
			eventActions.insert(std::make_pair("display", "You killed the lion! You hear a lion cub cry out for its father while another lion with scars and a dark mane roars in approval!"));
			eventActions.insert(std::make_pair("remove item", "lion"));
			eventActions.insert(std::make_pair("add item", "lion pelt"));
			return true;
		}

		else if (hParser.getAction().compare("give") == 0 && hParser.getObject().compare("lion pelt") == 0 && existsArr[HOLDING_OBJ] && hParser.getIndirect().compare("king") == 0 && currentRoom.compare("throne") == 0) {
			eventActions.insert(std::make_pair("change state", "lerna"));
			//eventActions.insert(std::make_pair("drop item", "lion pelt"));
			//eventActions.insert(std::make_pair("remove item", "lion pelt"));
			eventActions.insert(std::make_pair("add exit", "south"));
			eventActions.insert(std::make_pair("south", "lerna"));
			eventActions.insert(std::make_pair("display", "GAH! You actually killed it! How did you...!?! I mean, of course you did. I am such a compassionate ruler that I gave you a very easy task. Here is another simple labor for you to perform, go kill the hydra to the south! And take that scary, I mean, disgusting lion skin with you!"));
            eventActions.insert(std::make_pair("change short", "BIGLY, YUUGE PLACE!\nExits North and South.\n"));
            eventActions.insert(std::make_pair("change long", "This is the throne of King E. In the center of the rooms sits a golden throne.\nThe border wall to the south has been torn down!\n"));
			eventActions.insert(std::make_pair("change talk", "king"));
			eventActions.insert(std::make_pair("king", "Do you not understand Greek? Go kill the hydra of Lerna! Oh and your cousin wanted to talk to you about some nonsense now that the lion is gone.  He went for a walk."));
			return true;

		}

	}

    else if (currentLabor == LERNA){
		eventActions.insert(std::make_pair("display", ""));

		if (hParser.getAction().compare("look") == 0 && hParser.getObject().compare("shimmer") == 0 && existsArr[OBJ_EXISTS] && currentRoom.compare("swamp2") == 0) {
			eventActions["display"].append("This is your lucky day!  Someone dropped a coin here.");
			eventActions.insert(std::make_pair("add item", "coin"));
			eventActions.insert(std::make_pair("remove item", "shimmer"));
			return true;
		}

		else if (hParser.getAction().compare("give") == 0 && hParser.getObject().compare("coin") == 0 && existsArr[HOLDING_OBJ] && hParser.getIndirect().compare("iolaus") == 0 && currentRoom.compare("trail") == 0) {
			eventActions.insert(std::make_pair("drop item", "coin"));
			eventActions.insert(std::make_pair("remove item", "coin"));
			eventActions["display"].append("You found it!  Take my torch as thanks!");
			eventActions.insert(std::make_pair("add item", "torch"));
			return true;
		}

		if (hParser.getAction().compare("get") == 0 && hParser.getObject().compare("sword hilt") == 0 && existsArr[OBJ_EXISTS] && currentRoom.compare("swamp3") == 0) {
			eventActions["display"].append("\"You have not proven thyself worthy\", a voice booms.  You are zapped back to the palace");
			eventActions.insert(std::make_pair("move", "throne"));
			return true;
		}

		if (hParser.getAction().compare("say") == 0 && hParser.getObject().compare("fire") == 0 && gameData["swordUnlocked"].compare("false") == 0){
			eventActions["display"].append("The sword slides out of the stone and flies into your hand!");
			eventActions.insert(std::make_pair("remove item", "sword hilt"));
			eventActions.insert(std::make_pair("get item", "sword"));
			eventActions.insert(std::make_pair("change talk", "gossip"));
			eventActions.insert(std::make_pair("gossip", "The hydra hates smoke and where there's smoke, there's fire!"));
			gameData["swordUnlocked"] = "true";
			return true;
		}


		if (gameData["attackedHead"].compare("true") == 0) {
			if ((hParser.getAction().compare("use") == 0 && hParser.getObject().compare("torch") == 0 && existsArr[HOLDING_OBJ]
				&& hParser.getIndirect().compare("hydra") == 0 && existsArr[IND_EXISTS]) ||
				(hParser.getAction().compare("light") == 0 && hParser.getObject().compare("hydra") == 0 && existsArr[OBJ_EXISTS]
				&& hParser.getIndirect().compare("torch") == 0 && existsArr[HOLDING_IND])
				) {
				eventActions["display"].append("You hold the torch into the stump of the head you just knocked off. You hear the sizzling of flesh being seared and the smell of burnt flesh hits your nostrils. ");

				// check if player destroyed last hydra head
				if (atoi(gameData["numHydraHeads"].c_str()) < 1) {
					eventActions["display"].append("\nWith the last head finally gone and unable to regrow anymore the headless body of the hydra finally collapses onto the muddy ground.");
					eventActions.insert(std::make_pair("remove item", "hydra"));
					eventActions.insert(std::make_pair("add item", "hydra corpse"));
					eventActions.insert(std::make_pair("change long", "You are in the swamp of what was once the lair of the hydra.\n"));
					gameData["hydraUnlocked"] = "null";
				}
				else {
					eventActions["display"].append("The hyrda writhes in pain and begins to attack you again only this time with one less head.");
				}
				gameData["attackedHead"] = "false";

				return true;
			}
			else {
				eventActions["display"].append("The stump of the head you just knocked off begins quivering and blood starts oozing out. It is as if there is something trying to fight its way out of the hydra's body. You can only watch in shock as another head sprouts out of the stump. However, the stump begins quivering again and you look dumbfounded as ANOTHER head grows out of the same spot.\n");
				int numHydraHeads = atoi(gameData["numHydraHeads"].c_str()) + 2;
				std::stringstream ss;
				ss << numHydraHeads;
				ss >> gameData["numHydraHeads"];
			}

			gameData["attackedHead"] = "false";
		}


	    if (hParser.getAction().compare("use") == 0 &&
				hParser.getObject().compare("sword") == 0 && existsArr[HOLDING_OBJ] && hParser.getIndirect().compare("trees") == 0  && existsArr[IND_EXISTS]) {
			eventActions["display"].append("You manage to cut some branches off the trees and onto the ground.");
			eventActions.insert(std::make_pair("add item", "branches"));
			return true;
		}

 	    if (hParser.getAction().compare("attack") == 0 &&
				hParser.getObject().compare("trees") == 0 && existsArr[OBJ_EXISTS] && hParser.getIndirect().compare("sword") == 0 && existsArr[HOLDING_IND]) {
			eventActions["display"].append("You manage to cut some branches off the trees and onto the ground.");
			eventActions.insert(std::make_pair("add item", "branches"));
			return true;
		}

            if (hParser.getAction().compare("cut") == 0 &&
				hParser.getObject().compare("trees") == 0 && existsArr[OBJ_EXISTS] && hParser.getIndirect().compare("sword") == 0 && existsArr[HOLDING_IND]) {
			eventActions["display"].append("You manage to cut some branches off the trees and onto the ground.");
			eventActions.insert(std::make_pair("add item", "branches"));
			return true;
		}

		// if herc uses torch on tree branches in swamp4 then unlock hydra
		if (hParser.getAction().compare("use") == 0 && hParser.getObject().compare("torch") == 0 && existsArr[HOLDING_OBJ]
			&& hParser.getIndirect().compare("branches") == 0 && existsArr[HOLDING_IND]) {
				if  (currentRoom.compare("swamp4") == 0 && gameData["hydraUnlocked"].compare("false") == 0) {
					eventActions["display"].append("You light the branches with the torch and pretty soon you have a small fire going. The smoke begins billowing around the entrance of the den. After a few minutes you hear loud hissing and a large scaly beast slithers out of the den. THE HYDRA HAS APPEARED! \n\nThe hydra quickly slithers away heading further into the swamp. You jump into its path to prevent it from getting away. At that point the hydra wraps its tail around your one of your leg and rears its heads back baring its fangs.\n");
					gameData["hydraUnlocked"] = "true";
					eventActions.insert(std::make_pair("add item", "hydra"));
					eventActions.insert(std::make_pair("consume item", "branches"));
					eventActions.insert(std::make_pair("change long", "You are in a battle of life and death with the hydra! This is not the time to enjoy the scenery!\n"));
				}
				else {
					eventActions["display"].append("You light the branches with the torch and pretty soon you have a small fire going. You sit by the fire to warm yourself up and think about your journey so far.");
					eventActions.insert(std::make_pair("consume item", "branches"));
				}

			return true;
		}



            if (hParser.getAction().compare("light") == 0 && hParser.getObject().compare("branches") == 0 && existsArr[HOLDING_OBJ]
			&& hParser.getIndirect().compare("torch") == 0 && existsArr[HOLDING_IND]) {
				if  (currentRoom.compare("swamp4") == 0 && gameData["hydraUnlocked"].compare("false") == 0) {
					eventActions["display"].append("You light the branches with the torch and pretty soon you have a small fire going. The smoke begins billowing around the entrance of the den. After a few minutes you hear loud hissing and a large scaly beast slithers out of the den. THE HYDRA HAS APPEARED! \n\nThe hydra quickly slithers away heading further into the swamp. You jump into its path to prevent it from getting away. At that point the hydra wraps its tail around your one of your leg and rears its heads back baring its fangs.\n");
					gameData["hydraUnlocked"] = "true";
					eventActions.insert(std::make_pair("add item", "hydra"));
					eventActions.insert(std::make_pair("consume item", "branches"));
					eventActions.insert(std::make_pair("change long", "You are in a battle of life and death with the hydra! This is not the time to enjoy the scenery!\n"));
				}
				else {
					eventActions["display"].append("You light the branches with the torch and pretty soon you have a small fire going. You sit by the fire to warm yourself up and think about your journey so far.");
					eventActions.insert(std::make_pair("consume item", "branches"));
				}

			return true;
		}


		if (hParser.getAction().compare("move") == 0 && gameData["hydraUnlocked"].compare("true") == 0) {
			eventActions["display"].append("The hydra's tail is wrapped tightly around one of your legs preventing you from running.");
			return true;
		}

		if (hParser.getAction().compare("attack") == 0 && hParser.getObject().compare("hydra") == 0 && existsArr[OBJ_EXISTS]
			&& hParser.getIndirect().compare("sword") == 0 && existsArr[HOLDING_IND]) {

			eventActions["display"].append("You swing your sword with all your might at one of the hydras head and manage to slice it off. You pause to look with satisfaction at the bloody stump on the hydra's body.");

			// check if newly dispatched head is immortal
			int numHydraHeads = atoi(gameData["numHydraHeads"].c_str());
				//cout << "numHydraHeads = " << gameData["numHydraHeads"] << endl;
			if (gameData["attackedImmortalHead"].compare("false") == 0) {
				if ((rand() % numHydraHeads) == 0) {
					eventActions["display"].append("\n\nYou hear hissing at your feet. When you look down you see the head you just decapitated is still hissing and biting at you. You step on it with your only unencumbered foot and turn back to face the rest of the hydra's heads.");
					eventActions.insert(std::make_pair("add item", "hydra head"));
					gameData["attackedImmortalHead"] = "true";
				}
			}
			numHydraHeads--;
			std::stringstream ss;
			ss << numHydraHeads;
			ss >> gameData["numHydraHeads"];
			//cout << "numHydraHeads = " << gameData["numHydraHeads"] << endl;
			gameData["attackedHead"] = "true";

			return true;
		}


				if (hParser.getAction().compare("use") == 0 && hParser.getObject().compare("sword") == 0 && existsArr[HOLDING_OBJ]
			&& hParser.getIndirect().compare("hydra") == 0 && existsArr[IND_EXISTS]) {

			eventActions["display"].append("You swing your sword with all your might at one of the hydras head and manage to slice it off. You pause to look with satisfaction at the bloody stump on the hydra's body.");

			// check if newly dispatched head is immortal
			int numHydraHeads = atoi(gameData["numHydraHeads"].c_str());
				//cout << "numHydraHeads = " << gameData["numHydraHeads"] << endl;
			if (gameData["attackedImmortalHead"].compare("false") == 0) {
				if ((rand() % numHydraHeads) == 0) {
					eventActions["display"].append("\n\nYou hear hissing at your feet. When you look down you see the head you just decapitated is still hissing and biting at you. You step on it with your only unencumbered foot and turn back to face the rest of the hydra's heads.");
					eventActions.insert(std::make_pair("add item", "hydra head"));
					gameData["attackedImmortalHead"] = "true";
				}
			}
			numHydraHeads--;
			std::stringstream ss;
			ss << numHydraHeads;
			ss >> gameData["numHydraHeads"];
			//cout << "numHydraHeads = " << gameData["numHydraHeads"] << endl;
			gameData["attackedHead"] = "true";

			return true;
		}


        if (hParser.getAction().compare("look") == 0 && hParser.getObject().compare("shimmer") == 0
			&& existsArr[OBJ_EXISTS] && currentRoom.compare("swamp2") == 0) {
			eventActions["display"].append("This is your lucky day!  Someone dropped a coin here.");
			eventActions.insert(std::make_pair("add item", "silver coin"));
			eventActions.insert(std::make_pair("remove item", "shimmer"));
			return true;
		}

		if (hParser.getAction().compare("give") == 0 && hParser.getObject().compare("silver coin") == 0 && existsArr[HOLDING_OBJ]
			&& hParser.getIndirect().compare("iolaus") == 0 && currentRoom.compare("trail") == 0) {
			eventActions.insert(std::make_pair("drop item", "silver coin"));
			eventActions.insert(std::make_pair("remove item", "silver coin"));
			eventActions["display"].append("You found it!  Take my torch as thanks!");
			eventActions.insert(std::make_pair("add item", "torch"));
			return true;
		}

		if (hParser.getAction().compare("give") == 0 && hParser.getObject().compare("hydra head") == 0 && existsArr[HOLDING_OBJ]
				&& hParser.getIndirect().compare("king") == 0 && currentRoom.compare("throne") == 0) {
			eventActions.insert(std::make_pair("change state", "ceryneia"));
			eventActions.insert(std::make_pair("drop item", "hydra head"));
			eventActions.insert(std::make_pair("remove item", "hydra head"));
			eventActions.insert(std::make_pair("add exit", "west"));
			eventActions.insert(std::make_pair("west", "stymphalus"));
			eventActions["display"].append("Hmmm? What did you bring to me? WHAH! What is that?!? It's still biting!! Why is it still alive! FINE! FINE! I believe that you killed the hydra just keep that monstrosity away from me! However, I heard from the oracle of Hera that your cousin aided you in this labour. These labours are meant to be performed by you without any help so I am afraid that killing the hydra was completely meaningless. However, being the wise king that I am, I have already decided on your next labour. You are to bring back the hind of Ceryneian. I've ordered the West gate to be opened for you.");
			eventActions.insert(std::make_pair("change short", "BIGLY, YUUGE PLACE!\n"
										  "Exits North and South. The gate to the west leads to the town of Stymphalus.\n"));
			eventActions.insert(std::make_pair("change long", "This is the throne of King E. In the center of the rooms sits a golden throne.\nThe border wall to the south had been torn down and the gate leading west to Stymphalus has been opened.\n"));
			eventActions.insert(std::make_pair("change talk", "king"));
			eventActions.insert(std::make_pair("king", "Oh? Did you bring back the hind of Ceryneia already? No? Then I have more important matters to attend to."));

			return true;

		}


    }

		/*
		if (hParser.getAction().compare("give") == 0 && hParser.getObject().compare("hydra head") == 0 && existsArr[HOLDING_OBJ] && hParser.getIndirect().compare("king") == 0 && currentRoom.compare("throne") == 0) {
			eventActions.insert(std::make_pair("change state", "ceryneia"));
			//open exit in nemean room north to ceryneia. load rooms without text output
			current = loadRoom(itemList, roomItems, "nemean", 0);
			current.setExits(0, "ceryneia");
			current.setDescShort("The countryside of Nemea. North - Ceryneia; South - King Eurystheus\nWest - Lion's Cave; Down - Trail");
			current.setDescLong("You are standing above a canyon just outside the city-state of Nemea.\nThe road north leads to Ceryneia. If you go west you'll reach the lion's cave.\nThere also appears to be a trail down the canyon. Mycenae and the\nthrone room of King Eurystheus lie to the south.");
			current.setVisited(false);
			saveRoom(roomItems, current);
			//go back to throne room to continue adventure
			current = loadRoom(itemList, roomItems, currentRoom, 0);
			eventActions.insert(std::make_pair("display", "How in heavens did you kill that thing? On second thought, don't answer that.\nSince you clearly are adept at killing things, for this next labor I want you to bring me\nback something alive. Go north to Ceryneia and capture the fabled Hind and return it here."));
			eventActions.insert(std::make_pair("change long", "This is the throne of King E. In the center of the rooms sits a golden throne.\nYou can leave town along the road either north or south.\n"));
			eventActions.insert(std::make_pair("change talk", "king"));
			eventActions.insert(std::make_pair("king", "The Hind won't catch itself. Get going."));
			return true;
		}
		*/



	else if (currentLabor == CERYNEIA){

		if (hParser.getAction().compare("give") == 0 && hParser.getObject().compare("lion pelt") == 0 && existsArr[HOLDING_OBJ] && (hParser.getIndirect().compare("townswoman") == 0 || hParser.getIndirect().compare("arachne") == 0) && currentRoom.compare("ceryneia") == 0) {
			eventActions.insert(std::make_pair("display", "\n\"Well, Arachne. You say you're a seamstress. Can you make anything out of this?\"\n\n\"Is that real lion?!? I haven't had material this nice since, well, ever.\nWait right here. I'll be back shortly.\"\n\nArachne returns with a fabulous lion skin vest which fits like a glove\nand a long coil of rope woven from the lion's mane. \"This rope sure might\ncome in handy\" you think as you slide on the vest and thank Arachne."));
			eventActions.insert(std::make_pair("drop item", "lion pelt"));
			eventActions.insert(std::make_pair("remove item", "lion pelt"));
			eventActions.insert(std::make_pair("get item", "rope"));
			return true;
		}

		else if (currentRoom.compare(gameData["hindLocation"]) == 0 && currentRoom.compare("woods3") == 0 && gameData["trapSet"].compare("true") == 0) {
			gameData["hindLocation"] = "caught";
			eventActions.insert(std::make_pair("display", "\nAgain the Hind hears you approach and flees. However, unfortunately\nfor the Hind it scampers towards the narrow path where your trap is\nset. You hear a commotion amongst the brush followed by a shrill yell\nfrom the Hind and are relieved\nto see the Hind dangling from the air with a hind\nleg caught in the snare. You rush over to the animal and quickly use the\nremaining rope to hog tie it before throwing the captured creature over your shoulder\nto haul back to Mycenae and King Eurystheus."));
			eventActions.insert(std::make_pair("get item", "hind"));
			return true;
		}

		else if ((currentRoom.compare("woods3") == 0 || currentRoom.compare("woods4") == 0) && hParser.getAction().compare("use") == 0 && hParser.getObject().compare("rope") == 0 && existsArr[HOLDING_OBJ]) {
			if (gameData["trapSet"].compare("false") == 0) {
				gameData["trapSet"] = "true";
				eventActions.insert(std::make_pair("display", "\nTired of chasing this Hind all through these woods, you decide\nit's time to start hunting smarter and pull out the rope from Arachne.\nYou walk over to the narrow path and set up a snare to grab any unsuspecting passerby."));
			} else { //trap already set
				eventActions.insert(std::make_pair("display", "\nHow many traps do you think you need to set?\nIf you put up anymore you may not get off this trail without catching yourself.\nBesides, you've used all your rope, knucklehead."));
			}
			return true;
		}

		else if ((currentRoom.compare("woods3") == 0 || currentRoom.compare("woods4") == 0) && hParser.getAction().compare("set") == 0 && (hParser.getObject().compare("trap") == 0 || hParser.getObject().compare("snare") == 0)) {
			if (isItemPresent("rope", inventory)) {
				if (gameData["trapSet"].compare("false") == 0) {
					gameData["trapSet"] = "true";
					eventActions.insert(std::make_pair("display", "\nTired of chasing this Hind all through these woods, you decide\nit's time to start hunting smarter and pull out the rope from Arachne.\nYou walk over to the narrow path and set up a snare to grab any unsuspecting passerby."));
				} else { //trap already set
					eventActions.insert(std::make_pair("display", "\nHow many traps do you think you need to set?\nIf you put up anymore you may not get off this trail without catching yourself.\nBesides, you've used all your rope, knucklehead."));
				}
			} else {
				eventActions.insert(std::make_pair("display", "\nGreat idea! One problem, however...\nYou need the materials to do it."));
			}
			return true;
		}

		else if (currentRoom.compare(gameData["hindLocation"]) == 0) { //in room with Hind
			eventActions.insert(std::make_pair("display", "\nWhat's that? In the distance you see the glint of sunlight on gold.\nThe Hind heard your approach and scampered away before you\ncould get in range of a shot."));
			if (gameData["hindLocation"].compare("woods1") == 0) {
				gameData["hindLocation"] = "woods2";
			} else if (gameData["hindLocation"].compare("woods2") == 0) {
				gameData["hindLocation"] = "woods3";
			} else if (gameData["hindLocation"].compare("woods3") == 0) {
				gameData["hindLocation"] = "woods4";
			} else if (gameData["hindLocation"].compare("woods4") == 0) {
				gameData["hindLocation"] = "woods5";
			} else if (gameData["hindLocation"].compare("woods5") == 0) {
				gameData["hindLocation"] = "woods6";
			} else { //gameData["hindLocation"].compare("woods6") == 0)
				gameData["hindLocation"] = "woods1";
			}
			return true;
		}

		else if (hParser.getAction().compare("give") == 0 && hParser.getObject().compare("hind") == 0 && existsArr[HOLDING_OBJ] && hParser.getIndirect().compare("king") == 0 && currentRoom.compare("throne") == 0) {
			eventActions.insert(std::make_pair("change state", "erymanthia"));
			eventActions.insert(std::make_pair("add exit", "east"));
			eventActions.insert(std::make_pair("east", "tiryns"));
			eventActions.insert(std::make_pair("display", "Back so soon? With the Hind?! There appears to be no stopping you when you\nset your mind to something. Well, maybe this next task can flummox you. I need\nyou to go to Mount Erymanthian and capture the boar that is ravaging the nearby\nvillages. Think you can do that? If so, head east to the port at Tiryns to catch\na ride aboard any of the vessels heading in that direction. Oh, and here's\nsome coin to pay your way. Don't get used to my coin in your pocket, though.\nBe off!"));
			eventActions.insert(std::make_pair("drop item", "hind"));
			eventActions.insert(std::make_pair("remove item", "hind"));
			eventActions.insert(std::make_pair("get item", "coin"));
			eventActions.insert(std::make_pair("get item", "coin"));
			eventActions.insert(std::make_pair("get item", "coin"));
			eventActions.insert(std::make_pair("get item", "coin"));
			eventActions.insert(std::make_pair("get item", "coin"));
            eventActions.insert(std::make_pair("change short", "BIGLY, YUUGE PLACE!\nExits North, South, and East.\n"));
            eventActions.insert(std::make_pair("change long", "This is the throne of King E. In the center of the rooms sits a golden throne.\nThe road north leads to Nemea and Ceryneia; the road south leads to Lerna; and\nto the east is the port of Tiryns\n"));
			eventActions.insert(std::make_pair("change talk", "king"));
			eventActions.insert(std::make_pair("king", "I don't want to hear excuses. Bring me that boar!"));
			return true;
		}
    }

	return false;
}

void setLabor(LABORS &currentLabor, string newLabor) {
	// enum LABORS {NEMEAN, LERNA, CERYNEIA, ERYMANTHIA};
	if (newLabor.compare("lerna") == 0) {
		currentLabor = LERNA;
	} else if (newLabor.compare("ceryneia") == 0) {
		currentLabor = CERYNEIA;
	} else if (newLabor.compare("erymanthia") == 0) {
		currentLabor = ERYMANTHIA;
	}
}

bool isItemPresent(string itemName, std::map<string, Item*>& inventory, std::map<string, Item*>& roomItems) {
	// check if itemName in inventory
	if(inventory.find(itemName) != inventory.end()) {
		return true;
	}
	// if "club" not in inventory then check if in the room
	else if(roomItems.find(itemName) != roomItems.end()) {
		// check if the item is unlocked
		//if (roomItems[itemName]->isAvailable())
		return true;
	}
	else
		return false;
}


bool isItemPresent(string itemName, std::map<string, Item*>& inventory) {
	// check if itemName in inventory only
	if(inventory.find(itemName) != inventory.end()) {
		return true;
	}
	else
		return false;
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


void saveCurrentRoom(Room current) {
	string save_path = "./save/currentRoom.currentRoom";
	std::ofstream dest(save_path.c_str(), std::ios::binary);
	dest << current.getName();
	dest.close();
}


void scanDirectory(std::vector<string>& dir_contents, string dir_path) {
	DIR *dir = NULL;
	struct dirent *drnt = NULL;

	dir_contents.clear();
	dir = opendir(dir_path.c_str());
	if (dir) {
		while(drnt = readdir(dir)) {
			if ((strcmp(drnt->d_name, ".") != 0) && (strcmp(drnt->d_name, "..")) != 0) {
				dir_contents.push_back(drnt->d_name);
			}
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
	return modified;
}


void saveGame(std::map<string, Item*>& roomItems, Room current, std::map<string, Item*>& inventory, LABORS currentLabor, string& saveAs, std::map<string, string>& gameData) {
	int i, j, k;
	int save = 1, overwrite = 1, validInput = 1, modified = 1;
	string yes_no;

	//search through saved_games directory for names of saved games and store them in a vector
	std::vector<string> game_names;
	scanDirectory(game_names, "saved_games");

	//save current room, inventory, and labor to save folder before copying
	saveRoom(roomItems, current);
	saveCurrentRoom(current);
	saveInventory(inventory);
	saveLabor(currentLabor);
	saveGameData(gameData);

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


void loadGame(std::map<string, Item*>& itemMap, std::map<string, Item*>& rmItems, Room& current, LABORS& currentLabor, std::map<string, Item*>& itemList, std::map<string, Item*>& inventory, string& saveAs, std::map<string, string>& gameData){
	int i, loadFrom, load = 1, all_digits = 0;
	string input;
	string::size_type sz;
	//search through saved_games directory for names of saved games and store them in a vector
	std::vector<string> game_names;
	scanDirectory(game_names, "saved_games");

	if (game_names.size() > 0) {
		//get user input for which game to load. validate and if bad input iterate until acceptable
		cout << "Please enter a number corresponding to the saved game you would like to load:\n";
		for (i = 0; i < game_names.size(); i++) {
			cout << i + 1 << ". " << game_names.at(i) << endl;
		}
		std::getline(cin, input);
		for (i = 0; i < input.length(); i++) {
			if (!isdigit(input[i])) {
				all_digits = 1;
			}
		}
		if (all_digits == 0) {
			loadFrom = std::atoi(input.c_str());
			if ((loadFrom <= game_names.size()) && (loadFrom > 0)) {
				load = 0;
			}
		}
		while (load == 1) {
			all_digits = 0;
			cout << "Your input was either not a number or not a number corresponding to a saved game. Please enter a number corresponding to the saved game you would like to load:\n";
			for (i = 0; i < game_names.size(); i++) {
				cout << i + 1 << ". " << game_names.at(i) << endl;
			}
			std::getline(cin, input);
			for (i = 0; i < input.length(); i++) {
				if (!isdigit(input[i])) {
					all_digits = 1;
				}
			}
			if (all_digits == 0) {
				loadFrom = std::atoi(input.c_str());
				if ((loadFrom <= game_names.size()) && (loadFrom > 0)) {
					load = 0;
				}
			}
		}

		//clear inventory, clear gameData, clear room items, remove all files from save/ directory, and copy all files from saved game to save/ directory
		inventory.clear();
		gameData.clear();
		rmItems.clear();
		string remove = "rm -r -f save/";
		system(remove.c_str());
		string copy = "cp -r saved_games/";
		copy.append(game_names.at(loadFrom-1));
		copy.append("/ save/");
		system(copy.c_str());

		cout << "Loaded game: " << game_names.at(loadFrom-1) << endl;
		saveAs = game_names.at(loadFrom-1);

		//load inventory
		loadInventory(itemList, inventory);

		//load gameData
		string data_path = "./save/gameData.data";
		loadGameData(gameData, data_path);

		//load current room
		string current_room_name;
		string current_room_path = "./save/currentRoom.currentRoom";
		std::fstream current_room_file;
		current_room_file.open(current_room_path.c_str(), std::ios::out | std::ios::in);
		if (current_room_file) {
			//get each line of file which contains the current room name
			std::getline(current_room_file, current_room_name);
			current = loadRoom(itemList, rmItems, current_room_name, 0);
		} else {
			cout << "could not find/open current room file.\n";
		}
		current_room_file.close();

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << current.look();
		printRoomItem(rmItems);


		//load labor
		string current_labor_string;
		int current_labor_int;
		LABORS labors_list[] = {NEMEAN, LERNA, CERYNEIA, ERYMANTHIA};
		string current_labor_path = "./save/labor.labor";
		std::fstream current_labor_file;
		current_labor_file.open(current_labor_path.c_str(), std::ios::out | std::ios::in);
		if (current_labor_file) {
			//get each line of file which contains the current room name
			std::getline(current_labor_file, current_labor_string);
			current_labor_int = std::atoi(current_labor_string.c_str());;
			currentLabor = labors_list[current_labor_int];

			// change how the king speaks accordingly to the current labor
			string kingsTalk = "";
			switch (currentLabor) {
				case LERNA:
					kingsTalk = "Do you not understand Greek? Go kill the hydra of Lerna! Oh and your cousin wanted to talk to you about some nonsense now that the lion is gone.  He went for a walk.";
					break;
				case CERYNEIA:
					kingsTalk = "Oh? Did you bring back the hind of Ceryneia already? No? Then I have more important matters to attend to.";
					break;
				case ERYMANTHIA:
					kingsTalk = "I don't want to hear excuses. Bring me that boar!";
					break;
				default:
					kingsTalk = "Can someone tell this stinking, lumbering buffon to go kill the Nemean lion!";
					break;
			}

			itemMap["king"]->setTalk(kingsTalk);

		} else {
			cout << "could not find/open current labor file.\n";
		}
	} else {
		cout << "I'm sorry, but there are no saved games available to load.\n";
	}
}


void saveGameData(std::map<string, string>& gameData) {
	string save_path = "./save/gameData.data";
	std::ofstream dest(save_path.c_str(), std::ios::binary);
	for(map<string,string>::iterator it = gameData.begin(); it != gameData.end(); ++it) {
		if (it->first != "") {
			dest << it->first << endl << gameData[it->first] << endl;
		}
	}
	dest.close();
}


void loadGameData(std::map<string, string>& gameData, string dataFile) {
	string key = "";
	string data = "";
	std::fstream data_file;

	// convert dataFile to c-string for fstream open()
	data_file.open(dataFile.c_str(), std::ios::out | std::ios::in);

	if(data_file) {
		while(std::getline(data_file, data)) {
			key = data;
			std::getline(data_file, data);
			gameData.insert(std::make_pair(key, data));
			//cout << "[" << key << "] : [" << data << "]" << endl;
		}
	} else {
		cout << "Could not open data_file: " << dataFile << endl;
	}
}

void printGameData(std::map<string, string>& gameData) {
	cout << "Game Data:" << endl;
	for(map<string,string>::iterator it = gameData.begin(); it != gameData.end(); ++it) {
		cout << "[" << it->first << "] : [" << gameData[it->first] << "]" << endl;
	}
}


void checkForExit(Room R, Parser& P){

    string directions[6] = {"north", "south", "east", "west", "up", "down"};
    string reparse = "move ";

if (P.getAction().compare("move") == 0){

    for(int i = 0; i < NUM_OF_EXITS; i ++){
            if (P.getObject().compare(R.getDisplay_Exit(i)) == 0){
                reparse.append(directions[i]);
                P.parse(reparse);

                return;

            }
    }

}

else
        for(int i = 0; i < NUM_OF_EXITS; i ++){
            if (P.getAction().compare(R.getDisplay_Exit(i)) == 0){
            reparse.append(directions[i]);
            P.parse(reparse);

            return;

            }


    }

}


