/*
 * AJ Vrieland (ajv234)
 * Date: 3-3-21
 * CS232 Operating Systems
 * 
 */

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
    vector<string> userCommand;
};

#endif