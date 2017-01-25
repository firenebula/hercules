#ifndef PARSER_H
#define PARSER_H

#include <iostream>
using std::string;

class Parser
{
private:
    int action;
    int object;
    int indirect;
    void setAction(int action);
    void setObject(int object);
    void setIndirect(int indirect);

public:
    Parser();
    void parse(string inPut);
    int getAction() { return action; }
    int getObject() { return object; }
    int getIndirect()  { return indirect; }

};

#endif

