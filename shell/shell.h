#ifndef __CS232_Command_Shell__shell__
#define __CS232_Command_Shell__shell__

#include <iostream>
#include <string>
#include <vector>
#include "commandline.h"
#include "prompt.h"
#include "path.h"
using namespace std;

class Shell
{
public:
    Shell();
    void run();
private:
    string userInput;
    vector<string> userCommand;
    int myVar;
};

#endif