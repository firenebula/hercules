#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <string>
using std::string;


//ACTION DEFINITIONS



const string MOVE = "move";
string MOVE_CMDS[] = {"go", "move", "m"};

const string LOOK = "look";
string LOOK_CMDS[] = {"l", "look"};


//OBJECT DEFINITIONS

const string NORTH = "north";
string NORTH_CMDS[] = {"n", "north"};

const string SOUTH = "south";
string SOUTH_CMDS[] = {"s", "south"};

const string WEST = "west";
string WEST_CMDS[] = {"w", "west"};

const string EAST = "east";
string EAST_CMDS[] = {"e", "east"};





#endif


