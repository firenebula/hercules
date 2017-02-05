#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <string>
#include <vector>
#include <iterator>
using std::string;




//ACTION DEFINITIONS That use Objects

const string MOVE = "move";
const char* moveArr[] = {"go", "move"};
std::vector<std::string> MOVE_CMDS(moveArr, moveArr + 2);


const string LOOK = "look";
const char* lookArr[] = {"l", "look"};
std::vector<std::string> LOOK_CMDS(lookArr, lookArr + 2);


//Commands that stand for an action and an object

const string NORTH = "north";
string NORTH_CMDS[] = {"n", "north"};

const string SOUTH = "south";
string SOUTH_CMDS[] = {"s", "south"};

const string WEST = "west";
string WEST_CMDS[] = {"w", "west"};

const string EAST = "east";
string EAST_CMDS[] = {"e", "east"};





#endif


