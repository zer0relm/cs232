/*
 * AJ Vrieland (ajv234)
 * Date: 3-3-21
 * CS232 Operating Systems
 * 
 */

#ifndef __CS232_Command_Shell__path__
#define __CS232_Command_Shell__path__

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Path {
public:
    Path();
    string getDirectory(int index);
    int find(const string program);
private:
    void printPath();
    vector<string> pathVariable;
};

#endif