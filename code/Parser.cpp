#include "Parser.h"
#include "Definitions.h"


// Parser constructor
Parser::Parser()
{
    action = MOVE;
    object = NORTH;
    indirect = "inv";

}

void Parser::setAction(string action)
{
    this->action = action;
}


void Parser::setObject(string object)
{
    this->object = object;
}


void Parser::setIndirect(string indirect)
{
    this->indirect = indirect;
}


void Parser::parse(string inPut){

    action = NORTH;
    object = SOUTH;
    indirect = EAST;

}

void Parser::dictionaryLoad(){



}


void Parser::addEntry(map <string, string> curMap, string cmds[], string key){

}
