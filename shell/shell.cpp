#include "shell.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>

//#define DEBUGME 1



Shell::Shell()
{

    myVar = 1;
    userInput = "";
}

void Shell::run(){
    while(true){
        Prompt myPrompt;
        cout << myPrompt.get() << flush;
        CommandLine myCommandLine(cin);
        string myCommand = myCommandLine.getCommand();

#if DEBUGME
        cout << myCommand << flush;
#endif
        if (myCommand == "exit"){
            break;
        } else if( myCommand == "cd" ){
        cout << "TODO: cd <FileName>" << endl;
        }
    }
}