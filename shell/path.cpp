#include <unistd.h>
#include "path.h"
#include <stdlib.h>


Path::Path(){
    pathVariable.push_back(string(getcwd(NULL, 0)));
}