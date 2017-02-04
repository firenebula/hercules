#include "Parser.h"
#include "Definitions.h"


// Parser constructor
Parser::Parser()
{
    setAction("none");
    setObject("none");
    setIndirect("none");
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


void Parser::addEntry(map<string, string>& curMap, std::vector<string>& cmd, string value){



    for(unsigned int i = 0; i < cmd.size(); i++) {

        //std::cout << "current command: " << cmd[i] << " current value: " << value << std::endl;
      curMap.insert(std::make_pair(cmd[i], value));


    }

}

bool Parser::checkAction(string checkVal){


    //std::cout << "requested: " << checkVal << " retrieved: " << (actionMap.find(checkVal))->second << std::endl;


    bool found = true;
    map<string, string>::iterator it;
    it = actionMap.find(checkVal);
    //std::cout << "value returned" << it->second << std::endl;
    if(it != actionMap.end())
        setAction(it->second);
    else
        found = false;


    return found;




}





