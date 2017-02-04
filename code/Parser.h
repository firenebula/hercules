#ifndef PARSER_H
#define PARSER_H
#include <iterator>
#include <iostream>
#include <map>
#include <vector>
#include <string>
using std::string;
using std::map;


class Parser
{
private:
    string action;
    string object;
    string indirect;
    map  <string, string> actionMap;
    map  <string, string> ignoreMap;
    void setAction(string action);
    void setObject(string object);
    void setIndirect(string indirect);
    void dictionaryLoad();
    void addEntry(map <string, string> curMap, std::vector<string> cmds, string value);
    bool checkAction(string checkVal);

public:
    Parser();
    void parse(string inPut);
    string getAction() { return action; }
    string getObject() { return object; }
    string getIndirect()  { return indirect; }

};

#endif

