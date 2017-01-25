#include <iostream>
#include "Parser.h"
void testParseVal(string, Parser);

using std::cout;
using std::endl;

int main()
{
    Parser hParser;  // new Parser Object
    testParseVal("Default Values: ", hParser); // default values set to -1
    hParser.parse("any input"); // will parse input.  For now, just updating to dummy values
    testParseVal("After Parse: ", hParser);
    return 0;
}


// This test function is just printing out the contents of the Parse obj variables
void testParseVal(string label, Parser p){

    cout << label << endl << "Action Value: " << p.getAction() << endl
    << "Object Value: " << p.getObject() << endl
    << "Indirect Value: " << p.getIndirect() << endl;



}
