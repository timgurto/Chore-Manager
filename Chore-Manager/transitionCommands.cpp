// (C) 2016 Tim Gurto

#include <iostream>
#include <set>
#include <string>

#include "Sprint.h"

bool Sprint::checkEstimates(){
    // Make sure all estimates are in
    std::set<std::string> bludgers;
    for (const Chore &chore : _chores)
        for (const std::string &person : _people)
            if ((chore.owner() == "" || chore.owner() == person) &&
                 chore.estimate(person) == 0)
                bludgers.insert(person);
    if (bludgers.empty())
        return true;

    std::cout << "The following people still need to estimate some chores:";
    for (const std::string &name : bludgers)
        std::cout << ' ' << name;
    std::cout << std::endl;
    return false;
}
