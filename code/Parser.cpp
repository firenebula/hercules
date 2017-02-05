#include "Parser.hpp"
#include "Definitions.hpp"



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

    unsigned int i;
    std::vector<string> splitVec;
    splitWords(splitVec, inPut);
    bool doneProcessing = false;

    this->action = "none";

    for(i = 0; i < splitVec.size(); i++ && !doneProcessing){
        std::cout << "Word " << i + 1 << " : " << splitVec[i] << std::endl;
         if(checkAction(inPut))
            doneProcessing = true;

    }

//    checkAction(inPut);
//    if(!checkAction(inPut))
//    this->action = "none";

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


void Parser::splitWords(std::vector<string>& splitVec, string inPut){


     std::istringstream inStream(inPut);
    copy(std::istream_iterator<string>(inStream),
                 std::istream_iterator<string>(),
                 back_inserter(splitVec));



}




