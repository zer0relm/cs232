#include "shell.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>



Shell::Shell()
{

    myVar = 1;
    userInput = "";
}

void Shell::run(){
    while(true){
        cout << "-> " << flush;
        CommandLine myCommandLine(cin);
        cout << userInput << flush;
    }
}