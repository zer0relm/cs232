#include "shell.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>


Shell::Shell(){

    myVar = 1;
}

void Shell::run(){
    while(true){
        printf("hello world");
    }
}