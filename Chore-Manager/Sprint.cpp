// (C) 2016 Tim Gurto

#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Sprint.h"
#include "constants.h"

Sprint::Sprint(){

    // Get people
    std::list<std::string> people;
    {
        std::ifstream peopleFile("people.dat");
        std::string name;
        while (std::getline(peopleFile, name))
            people.push_back(name);
    }

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
    static const std::string HELP("Commands: begin chore estimate help list remove quit");
    std::cout << HELP << std::endl;

    bool beginning = false;

    while (true){
        std::string command;
        std::cout << std::endl << PROMPT;
        std::getline(std::cin, command);
        if (command == "begin"){ beginning = true; break;}
        else if (command == "chore") addChore();
        else if (command == "estimate") estimate();
        else if (command == "help") std::cout << HELP << std::endl;
        else if (command == "list") list();
        else if (command == "remove") remove();
        else if (command == "quit") break;
        else
            std::cout << "Invalid command." << std::endl;

        backup(); // After every command completes.
    }

    if (beginning){
        // Finalize and begin sprint
        resume();
    }
}


void Sprint::resume(){

}
