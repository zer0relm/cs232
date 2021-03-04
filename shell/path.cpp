/*
 * AJ Vrieland (ajv234)
 * Date: 3-3-21
 * CS232 Operating Systems
 * 
 */

#include <unistd.h>
#include "path.h"
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <iostream>
#include <dirent.h>

//#define DEBUGME 1


Path::Path(){
    string tempVar = string(getenv("PATH"));
    string tempWord = "";
    istringstream ss(tempVar);
    while(getline(ss, tempWord, '.')){
        pathVariable.push_back(tempWord);
        while (getline(ss, tempWord, ':')){
            if (tempWord == "local/bin"){continue;}
            pathVariable.push_back(tempWord);
        }
        
    }


}

string Path::getDirectory(int index){
    return pathVariable[index];
}

int Path::find(const string program){
    
    for (int i = pathVariable.size(); i > 0; i--){
#if DEBUGME
        cout << program << endl;
        //printPath();
#endif
        struct dirent *de;
        const char *pV =  " ";
        pV = pathVariable[i].c_str();
        DIR *dir = opendir(pV);
        cout << pV << endl;
        while ((de = readdir(dir)) != NULL){
            //cout << de->d_name << " == " << program << endl;
            if (de->d_name == program){
                return i;
            }
        }
    }
    return -1;
}

void Path::printPath(){
    for (int i = 0; i < pathVariable.size(); i++){
        cout << pathVariable[i] << endl;
    }
}