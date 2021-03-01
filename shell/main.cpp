//
//  main.cpp
//  CS232 Command Shell
//
//  Created by Victor Norman on 1/20/15.
//  Copyright (c) 2015 Victor Norman. All rights reserved.
//

#include <iostream>
#include "shell.h"
#include "commandline.h" //CLEANUP
#include "path.h" //ClEANUP

//int main(int argc, const char *argv[])
int main()
{
    Shell myShell;
    myShell.run();
    Path myPath;
    //cout << myPath.getPath() << flush;
}
