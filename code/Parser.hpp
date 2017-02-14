#ifndef PARSER_H
#define PARSER_H
#include <iterator>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using std::string;
using std::map;



class Parser
{
private:
    string action;
    string object;
    string indirect;
    map  <string, string> actionMap;
    map  <string, string> actMovMap;


    void setAction(string action);
    void setObject(string object);
    void setIndirect(string indirect);
    void dictionaryLoad();
    void addEntry(map <string, string>& curMap, std::vector<string>, string value);
    bool checkAction(string checkVal);
    bool checkMovObj(string checkVal);
    void splitWords(std::vector<string>& splitVec, string inPut);
    bool checkIgnore(string checkVal);

public:
    Parser();
    void parse(string inPut);
    string getAction() { return action; }
    string getObject() { return object; }
    string getIndirect()  { return indirect; }

};

#endif

