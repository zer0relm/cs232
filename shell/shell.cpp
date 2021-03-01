#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>

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
            if (myCommandLine.getArgCount() == 2){
                string pathAppend = myCommandLine.getArgVector(1);
                const char *nP = "           ";
                nP = pathAppend.c_str();
                char *newPath = getcwd(NULL, 0);
                strcat(newPath, "/");
                strcat(newPath, nP);
                cout << newPath << endl;
                chdir(newPath);
                myPrompt.set();
            }
        } else if( myCommand == "pwd"){
            cout << string(getcwd(NULL, 0)) << endl;
        }
    }
}