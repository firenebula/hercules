#include "Parser.h"
#include "Definitions.h"


// Parser constructor
Parser::Parser()
{
    action = "none";
    object = "none";
    indirect = "none";
    dictionaryLoad();

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


    checkAction(inPut);
    if(!checkAction(inPut))
    this->action = "none";

}

void Parser::dictionaryLoad(){

    addEntry(actionMap, MOVE_CMDS, MOVE);
    addEntry(actionMap, LOOK_CMDS, LOOK);

}


void Parser::addEntry(map <string, string> curMap, std::vector<string> cmd, string value){



    for(int i = 0; i < cmd.size(); i++) {

        curMap.insert(std::pair<string, string>(cmd[i], value));

    }

}

bool Parser::checkAction(string checkVal){

    bool found = true;
    std::map<string, string>::iterator it;
    it = actionMap.find(checkVal);
    if(it != actionMap.end())
        setAction(it->second);
    else
        found = false;

    return found;




}





