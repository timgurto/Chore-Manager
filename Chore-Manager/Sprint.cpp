// (C) 2016 Tim Gurto

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Sprint.h"
#include "constants.h"

Sprint::Sprint(){

    /* Check current sprint:
    If no start date, none is in progress; resume setting it up.
    If end date has expired, archive it and start setting up a new one.
    If valid start and end date, resume progress.
    */

    // 1. If no start date, none is in progress; resume setting it up.
    {
        std::ifstream startFile("sprint/start.dat");
        if (!startFile){
            // Resume setup
            _inProgress = false;
            return;
        }
        startFile >> _startTime;
    }

    // 2. If end date has expired, archive it and start a new one.
    // Assumption: valid start/end files and times.
    {
        std::ifstream endFile("sprint/end.dat");
        endFile >> _endTime;
    }
    time_t currentTime;
    time(&currentTime);
    if (currentTime > _endTime){

        // Archive current files
        std::ostringstream oss;
        oss << _startTime << " " << _endTime;
        std::string dirName = oss.str();
        rename("sprint/start.dat", (dirName + "/start.dat").c_str());
        rename("sprint/end.dat", (dirName + "/end.dat").c_str());

        // Set up new sprint
        _inProgress = false;
        return;
    }
    
    // 3. Resume progress
    _inProgress = true;
}

void Sprint::setup(){

}

void Sprint::resume(){

}
