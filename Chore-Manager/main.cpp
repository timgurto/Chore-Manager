// (C) 2016 Tim Gurto

#include <iostream>
#include <string>

#include "Sprint.h"
#include "constants.h"

int main(){

    Sprint sprint;

    if (sprint.inProgress()){
        sprint.resume();
    }else{
        //Setup
    }
}
