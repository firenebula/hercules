#include "Parser.hpp"
#include "Definitions.hpp"



// Parser constructor
Parser::Parser()
{
    setAction(NONE);
    setObject(NONE);
    setIndirect(NONE);
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

    unsigned int i =0;
    setAction(NONE);
    setObject(NONE);
    setIndirect(NONE);
    string tempStr = "$none";

    std::vector<string> splitVec;
    splitWords(splitVec, inPut);
    bool doneProcessing = false;

    while(i < splitVec.size() && !doneProcessing){
       // std::cout << "Word " << i + 1 << " : " << splitVec[i] << std::endl;

            if(checkMovObj(splitVec[i]))
                doneProcessing = true;

            else if(checkAction(splitVec[i])){
                i++; //checking next word
                doneProcessing = true;
                while(i < splitVec.size() && tempStr == "$none"){
                    if(!checkIgnore(splitVec[i]))
                        tempStr = splitVec[i];
                    i++;
                }
                while(i < splitVec.size() && !checkIgnore(splitVec[i])){
                      tempStr += " ";
                      tempStr += splitVec[i];
                      i++;
                      }

                   setObject(tempStr);
                   tempStr = "$none";




             while(i < splitVec.size() && tempStr == "$none"){
                    if(!checkIgnore(splitVec[i]))
                        tempStr = splitVec[i];
                    i++;
                }
                while(i < splitVec.size() && !checkIgnore(splitVec[i])){
                      tempStr += " ";
                      tempStr += splitVec[i];
                      i++;
                      }

                   setIndirect(tempStr);

            }//end else if

                i++; // main loop incrementer

            }





    }

//    checkAction(inPut);
//    if(!checkAction(inPut))
//    this->action = "none";



void Parser::dictionaryLoad(){

    addEntry(actionMap, MOVE_CMDS, MOVE);
    addEntry(actionMap, LOOK_CMDS, LOOK);
    addEntry(actionMap, USE_CMDS, USE);
    addEntry(actionMap, DROP_CMDS, DROP);
    addEntry(actionMap, GET_CMDS, GET);
    addEntry(actionMap, ATTACK_CMDS, ATTACK);
    addEntry(actionMap, PUSH_CMDS,  PUSH);
    addEntry(actionMap, QUIT_CMDS,  QUIT);
    addEntry(actionMap, GIVE_CMDS,  GIVE);
    addEntry(actionMap, SHOOT_CMDS,  SHOOT);
    addEntry(actionMap, SAY_CMDS,  SAY);
    addEntry(actionMap, SET_CMDS,  SET);
    addEntry(actionMap, LIGHT_CMDS,  LIGHT);
    addEntry(actionMap, CUT_CMDS,  CUT);

    addEntry(actionMap, TALK_CMDS,  TALK);
    addEntry(actionMap, INVENTORY_CMDS,  INVENTORY);

    addEntry(actMovMap, NORTH_CMDS, NORTH);
    addEntry(actMovMap, SOUTH_CMDS, SOUTH);
    addEntry(actMovMap, WEST_CMDS, WEST);
    addEntry(actMovMap, EAST_CMDS, EAST);
    addEntry(actMovMap, UP_CMDS, UP);
    addEntry(actMovMap, DOWN_CMDS, DOWN);

    addEntry(actMovMap, START_CMDS,  START);



}


void Parser::addEntry(map<string, string>& curMap, std::vector<string> cmd, string value){



    for(unsigned int i = 0; i < cmd.size() ; i++) {

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
    if(it != actionMap.end()){
        setAction(it->second);

    }
    else
        found = false;


    return found;


}

bool Parser::checkMovObj(string checkVal){


    //std::cout << "requested: " << checkVal << " retrieved: " << (actionMap.find(checkVal))->second << std::endl;


    bool found = true;
    map<string, string>::iterator it;
    it = actMovMap.find(checkVal);
    //std::cout << "value returned" << it->second << std::endl;
    if(it != actMovMap.end()){
        setObject(it->second);
        setAction(MOVE);

    }
    else
        found = false;


    return found;


}


bool Parser::checkIgnore(string checkVal){


    //std::cout << "requested: " << checkVal << " retrieved: " << (actionMap.find(checkVal))->second << std::endl;
    bool found = false;

    if( std::find(IGNORE_WORDS.begin(), IGNORE_WORDS.end(), checkVal) != IGNORE_WORDS.end()){

        found = true;

    }


    return found;

}




void Parser::splitWords(std::vector<string>& splitVec, string inPut){


     std::istringstream inStream(inPut);
    copy(std::istream_iterator<string>(inStream),
                 std::istream_iterator<string>(),
                 back_inserter(splitVec));


    for (unsigned int i = 0; i < splitVec.size(); i++){
            for(unsigned int j = 0; j < splitVec[i].length(); j++) {
                splitVec[i][j] = tolower(splitVec[i][j]);
		}
    }


}




