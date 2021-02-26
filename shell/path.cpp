#include <unistd.h>
#include "path.h"
#include <stdlib.h>
#include <sstream>
#include <string.h>



Path::Path(){
    string tempVar = string(getenv("PATH"));
    for(int i = 0; i <= tempVar.length(); i++ ){
        if(tempVar[i] == '.'){
            
        }
    }
    pathVariable.push_back("");
}

string Path::getPath(){
    return pathVariable[0];
}