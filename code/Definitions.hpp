#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#include <string>
#include <vector>
#include <iterator>
using std::string;



const char* NONE = "$none";

//ACTION DEFINITIONS That use Objects
const string SET = "set";
string setArr[] = {"set", "make"};
std::vector<std::string> SET_CMDS(setArr, setArr+ sizeof(setArr) / sizeof(setArr[0]));

const string LIGHT = "light";
string lightArr[] = {"light", "burn", "ignite"};
std::vector<std::string> LIGHT_CMDS(lightArr, lightArr+ sizeof(lightArr) / sizeof(lightArr[0]));

const string CUT = "cut";
string cutArr[] = {"cut", "break"};
std::vector<std::string> CUT_CMDS(cutArr, cutArr+ sizeof(cutArr) / sizeof(cutArr[0]));

const string EAT = "eat";
string eatArr[] = {"eat", "consume"};
std::vector<std::string> EAT_CMDS(eatArr, eatArr+ sizeof(eatArr) / sizeof(eatArr[0]));



const string GIVE = "give";
string giveArr[] = {"give"};
std::vector<std::string> GIVE_CMDS(giveArr, giveArr+ sizeof(giveArr) / sizeof(giveArr[0]));

const string SHOOT = "shoot";
string shootArr[] = {"shoot"};
std::vector<std::string> SHOOT_CMDS(shootArr, shootArr+ sizeof(shootArr) / sizeof(shootArr[0]));

const string MOVE = "move";
string moveArr[] = {"go", "move", "approach"};
std::vector<std::string> MOVE_CMDS(moveArr, moveArr+ sizeof(moveArr) / sizeof(moveArr[0]));

const string PUSH = "push";
string pushArr[] = {"push", "shove"};
std::vector<std::string> PUSH_CMDS(pushArr, pushArr+ sizeof(pushArr) / sizeof(pushArr[0]));

const string LOOK = "look";
string lookArr[] = {"l", "look", "examine"};
std::vector<std::string> LOOK_CMDS(lookArr, lookArr + sizeof(lookArr) / sizeof(lookArr[0]));

const string GET = "get";
string getArr[] = {"get", "take", "grab", "pickup"};
std::vector<std::string> GET_CMDS(getArr, getArr + sizeof(getArr) / sizeof(getArr[0]));


const string USE = "use";
string useArr[] = {"use", "activate", "swing"};
std::vector<std::string> USE_CMDS(useArr, useArr + sizeof(useArr) / sizeof(useArr[0]));

const string DROP = "drop";
string dropArr[] = {"drop", "leave", "putdown"};
std::vector<std::string> DROP_CMDS(dropArr, dropArr + sizeof(dropArr) / sizeof(dropArr[0]));

const string ATTACK = "attack";
string attackArr[] = {"hit", "kill", "attack", "bash"};
std::vector<std::string> ATTACK_CMDS(attackArr, attackArr + sizeof(attackArr) / sizeof(attackArr[0]));

const string TALK = "talk";
string talkArr[] = {"talk", "speak", "converse", "ask"};
std::vector<std::string> TALK_CMDS(talkArr, talkArr + sizeof(talkArr) / sizeof(talkArr[0]));

const string INVENTORY = "inventory";
string inventoryArr[] = {"inv", "inventory"};
std::vector<std::string> INVENTORY_CMDS(inventoryArr, inventoryArr + sizeof(inventoryArr) / sizeof(inventoryArr[0]));

const string QUIT = "quit";
string quitArr[] = {"q", "quit"};
std::vector<std::string> QUIT_CMDS(quitArr, quitArr + sizeof(quitArr) / sizeof(quitArr[0]));

const string HELP = "help";
string helpArr[] = {"h", "help", "?"};
std::vector<std::string> HELP_CMDS(helpArr, helpArr + sizeof(helpArr) / sizeof(helpArr[0]));

const string SAY = "say";
string sayArr[] = {"say", "speak"};
std::vector<std::string> SAY_CMDS(sayArr, sayArr + sizeof(sayArr) / sizeof(sayArr[0]));

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

const string UP = "up";
string upArr[] = {"u", "up"};
std::vector<std::string> UP_CMDS(upArr, upArr + sizeof(upArr) / sizeof(upArr[0]));

const string DOWN = "down";
string downArr[] = {"d", "down"};
std::vector<std::string> DOWN_CMDS(downArr, downArr + sizeof(downArr) / sizeof(downArr[0]));



//commands to ignore
const char* ignoreArr[] = {"the", "of", "a", "an", "to", "around", "at", "in", "on", "with", "from", "into", "onto"};
std::vector<std::string> IGNORE_WORDS(ignoreArr, ignoreArr + sizeof(ignoreArr) / sizeof(ignoreArr[0]));





#endif


