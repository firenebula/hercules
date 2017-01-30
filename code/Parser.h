#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
using std::string;
using std::map;


class Parser
{
private:
    string action;
    string object;
    string indirect;
    map  <string, string> actionMap;
    map  <string, string> objectMap;
    void setAction(string action);
    void setObject(string object);
    void setIndirect(string indirect);
    void dictionaryLoad();
    void addEntry(map <string, string> curMap, string cmds[], string key);

public:
    Parser();
    void parse(string inPut);
    string getAction() { return action; }
    string getObject() { return object; }
    string getIndirect()  { return indirect; }

};

#endif

