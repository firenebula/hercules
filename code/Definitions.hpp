#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <string>
#include <vector>
#include <iterator>
using std::string;



const char* NONE = "$none";

//ACTION DEFINITIONS That use Objects

const string MOVE = "move";
string moveArr[] = {"go", "move"};
std::vector<std::string> MOVE_CMDS(moveArr, moveArr+ sizeof(moveArr) / sizeof(moveArr[0]));

const string LOOK = "look";
string lookArr[] = {"l", "look"};
std::vector<std::string> LOOK_CMDS(lookArr, lookArr + sizeof(lookArr) / sizeof(lookArr[0]));

const string GET = "get";
string getArr[] = {"get", "take", "grab"};
std::vector<std::string> GET_CMDS(getArr, getArr + sizeof(getArr) / sizeof(getArr[0]));


const string USE = "use";
string useArr[] = {"use", "activate"};
std::vector<std::string> USE_CMDS(useArr, useArr + sizeof(useArr) / sizeof(useArr[0]));

const string DROP = "drop";
string dropArr[] = {"drop", "leave"};
std::vector<std::string> DROP_CMDS(dropArr, dropArr + sizeof(dropArr) / sizeof(dropArr[0]));

const string ATTACK = "attack";
string attackArr[] = {"hit", "kill", "attack", "bash", "shoot"};
std::vector<std::string> ATTACK_CMDS(attackArr, attackArr + sizeof(attackArr) / sizeof(attackArr[0]));

const string QUIT = "quit";
string quitArr[] = {"q", "quit"};
std::vector<std::string> QUIT_CMDS(quitArr, quitArr + sizeof(quitArr) / sizeof(quitArr[0]));

//Commands that stand for an action and an object



const string START = "start";
string startArr[] = {"start"};
std::vector<std::string> START_CMDS(startArr, startArr + sizeof(startArr) / sizeof(startArr[0]));



const string NORTH = "north";
string northArr[] = {"n", "north"};
std::vector<std::string> NORTH_CMDS(northArr, northArr + sizeof(northArr) / sizeof(northArr[0]));

const string SOUTH = "south";
string southArr[] = {"s", "south"};
std::vector<std::string> SOUTH_CMDS(southArr, southArr + sizeof(southArr) / sizeof(southArr[0]));

const string WEST = "west";
string westArr[] = {"w", "west"};
std::vector<std::string> WEST_CMDS(westArr, westArr + sizeof(westArr) / sizeof(westArr[0]));


const string EAST = "east";
string eastArr[] = {"e", "east"};
std::vector<std::string> EAST_CMDS(eastArr, eastArr + sizeof(eastArr) / sizeof(eastArr[0]));



//commands to ignore
const char* ignoreArr[] = {"the", "of", "a", "and", "to", "around", "at", "in", "on", "with"};
std::vector<std::string> IGNORE_WORDS(ignoreArr, ignoreArr + sizeof(ignoreArr) / sizeof(ignoreArr[0]));





#endif


