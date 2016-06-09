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
    {
        std::ifstream peopleFile("people.dat");
        std::string name;
        while (std::getline(peopleFile, name))
            _people.push_back(name);
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

            // Load existing data
            std::ifstream choresFile("sprint/chores.dat");
            while (choresFile){
                std::string line;
                std::getline(choresFile, line);
                std::istringstream iss(line);
                static const size_t BUF_SIZE = 100;
                char buffer[BUF_SIZE+1];
                iss.get(buffer, BUF_SIZE, DELIM);
                std::string name(buffer);
                iss.ignore();
                if (name == "")
                    continue;
                iss.get(buffer, BUF_SIZE, DELIM);
                std::string owner(buffer);
                if (owner == SHARED)
                    owner = "";
                Chore c(name, owner);
                for (const std::string &person : _people){
                    if (owner != SHARED)
                        iss.ignore();
                    size_t estimate;
                    iss >> estimate;
                    c.estimate(person, estimate);
                }
                _chores.insert(c);
            }

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
        oss << _startTime << "-" << _endTime;
        std::string dirName = oss.str();
        rename("sprint/start.dat", (dirName + "/start.dat").c_str());
        rename("sprint/end.dat", (dirName + "/end.dat").c_str());
        rename("sprint/chores.dat", (dirName + "/chores.dat").c_str());

        // Set up new sprint
        _inProgress = false;
        return;
    }
    
    // 3. Resume progress
    _inProgress = true;
}

void Sprint::setup(){
    static const std::string HELP(
        "Available commands:\n"
        "h, help      Display this help text\n"
        "a, add       Add a new chore\n"
        "r, remove    Remove a chore\n"
        "l, list      List all chores, and who has given them estimates\n"
        "e, estimate  Provide your estimates for chores\n"
        "b, begin     Begin the sprint, locking in estimates and distributing chores\n"
        "q, quit      Quit the program.  All data will be saved"
    );
    std::cout << HELP << std::endl;

    bool beginning = false;

    while (true){
        std::string command;
        std::cout << std::endl << PROMPT;
        std::getline(std::cin, command);
        if (     command == "b" || command == "begin")
            { if (checkEstimates()) {beginning = true; break;}}
        else if (command == "a" || command == "add")      addChore();
        else if (command == "e" || command == "estimate") estimate();
        else if (command == "h" || command == "help")     std::cout << HELP << std::endl;
        else if (command == "l" || command == "list")     list();
        else if (command == "r" || command == "remove")   remove();
        else if (command == "q" || command == "quit")     { backup(); break; }
        else
            std::cout << "Invalid command." << std::endl;

        backup(); // After every command completes.
    }

    if (beginning){
        // Finalize and begin sprint

        getEndTime();
        allocate();
        recordStartTime();

        resume();
    }
}


void Sprint::resume(){

}
