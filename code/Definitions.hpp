#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <string>
#include <vector>
#include <iterator>
using std::string;


const char* NONE = "$none";

//ACTION DEFINITIONS That use Objects

const string MOVE = "move";
const char* moveArr[] = {"go", "move"};
std::vector<std::string> MOVE_CMDS(moveArr, moveArr+ sizeof(moveArr) / sizeof(moveArr[0]));


const string LOOK = "look";
const char* lookArr[] = {"l", "look"};
std::vector<std::string> LOOK_CMDS(lookArr, lookArr + sizeof(lookArr) / sizeof(lookArr[0]));


//Commands that stand for an action and an object

const string NORTH = "north";
const char* northArr[] = {"n", "north"};
std::vector<std::string> NORTH_CMDS(northArr, northArr + sizeof(northArr) / sizeof(northArr[0]));

const string SOUTH = "south";
const char* SOUTH_CMDS[] = {"s", "south"};

const string WEST = "west";
const char* WEST_CMDS[] = {"w", "west"};

const string EAST = "east";
const char* EAST_CMDS[] = {"e", "east"};


//commands to ignore
const char* ignoreArr[] = {"the", "of", "a", "and"};
std::vector<std::string> IGNORE_WORDS(ignoreArr, ignoreArr + sizeof(ignoreArr) / sizeof(ignoreArr[0]));



#endif


