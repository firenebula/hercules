#include "Parser.h"

// Parser constructor
Parser::Parser()
{
    action = -1;
    object = -1;
    indirect = -1;

}

void Parser::setAction(int action)
{
    this->action = action;
}


void Parser::setObject(int object)
{
    this->object = object;
}


void Parser::setIndirect(int indirect)
{
    this->indirect = indirect;
}


void Parser::parse(string inPut){

    action = 1;
    object = 2;
    indirect = 3;

}

