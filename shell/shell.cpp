#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include "path.h"

//#define DEBUGME 1



Shell::Shell()
{

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
            // The cd funciton changes the current working directory by appending the file to the end of the path, and updating prompt and path
            // I did not explicitly implement .. (go back) in here but it still works somehow
            // It does not like going into some files, i.e. Homework02 from /home/zer0relm/cs232, but will go into other dirctories
            // The clutter is becuase I had to switch between string literal and c-string multiple times
            if (myCommandLine.getArgCount() >= 2){
                string pathAppend = myCommandLine.getArgVector(1);
                const char *nP = "           ";
                nP = pathAppend.c_str();
                char *newPath = getcwd(NULL, 0);
                strcat(newPath, "/");
                strcat(newPath, nP);
                chdir(newPath);
                myPrompt.set();
            }
        } else if( myCommand == "pwd"){
            cout << string(getcwd(NULL, 0)) << endl;
        } else if( myCommand == "ls"){
            // I implemented a ls command for fun (and for debuging), it's not great but it works
            struct dirent *de;
            DIR *dir = opendir(getcwd(NULL, 0));
            while ((de = readdir(dir)) != NULL){
                printf("%s,  ", de->d_name);
            }
            cout << endl;
        } else if(myCommand == "find"){
            // I created this to test the path.find() function
            // Prints the dirctory where a program is found
            Path myPath; 
            if (myCommandLine.getArgCount() >= 2){
                string program = myCommandLine.getArgVector(1);
                int directoryIndex = myPath.find(program);
                //printf("%f", directoryIndex);
                if( directoryIndex != -1){
                    cout << myPath.getDirectory(directoryIndex) << endl;
                }
            }
        }
    }
}