#include <unistd.h>
#include "path.h"
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <iostream>



Path::Path(){
    string tempVar = string(getenv("PATH"));
    string tempWord = "";
    // for(int i = 0; i <= tempVar.length(); i++ ){
    //     if(tempVar[i] == '.'){
            
    //     }
    // }
    istringstream ss(tempVar);
    while(getline(ss, tempWord, '.')){
        pathVariable.push_back(tempWord);
        while (getline(ss, tempWord, ':')){
            pathVariable.push_back(tempWord);
        }
        
    }


}

string Path::getPath(int index){
    return pathVariable[index];
}