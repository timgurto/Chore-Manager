// (C) 2016 Tim Gurto

#include <iostream>
#include <string>

#include "Sprint.h"
#include "constants.h"

void Sprint::addChore(){
    std::string name, owner;
    std::cout << "Chore description: ";
    std::getline(std::cin, name);
    std::cout << "Enter the person responsible for this chore, or leave blank for shared: ";
    std::getline(std::cin, owner);
    _chores.insert(Chore(name, owner));
}

void Sprint::estimate(){
    std::string name;
    std::cout << "Enter your name: " << PROMPT;
    std::getline(std::cin, name);
    bool found = false;
    for (const std::string &person : _people)
        if (person == name){
            found = true;
            break;
        }
    if (!found){
        std::cout << "Invalid name." << std::endl;
        return;
    }

    // Display list, with estimates
        size_t i = 0;
    for (const Chore &chore : _chores){
        std::cout << "  "
                  << (i <= 10 ? " " : "")
                  << (i <= 100 ? " " : "")
                  << ++i << "  " << chore.name();
        for (size_t j = chore.name().length(); j != 30; ++j)
            std::cout << ' ';
        size_t estimate = chore.estimate(name);
        if (estimate == 0)
            std::cout << "  -";
        else
            std::cout << (estimate < 10 ? " " : "")
                      << (estimate < 100 ? " " : "")
                      << (estimate == 0 ? '-' : estimate);
        std::cout << std::endl;
    }

    std::cout << "Please enter the number of the chore to estimate, or 0 to estimate all chores: " << PROMPT;
    size_t toEstimate;
    std::cin >> toEstimate;
    std::string dummy;
    std::getline(std::cin, dummy);

    if (toEstimate > _chores.size()){
        std::cout << "Invalid number" << std::endl;
        return;
    }

    if (toEstimate > 0){
        size_t i = 0;
        for (const Chore &choreConst : _chores)
            if (++i == toEstimate){

                Chore &chore = const_cast<Chore &>(choreConst);
                std::cout << "Estimate for \"" << chore.name() << "\": " << PROMPT;
                size_t estimate;
                std::cin >> estimate;
                std::getline(std::cin, dummy);
                chore.estimate(name, estimate);

                return;
            }
        return;
    }

    // Estimate all
    std::cout << "Enter an estimate for each item:" << std::endl;
    for (const Chore &choreConst : _chores){
        Chore &chore = const_cast<Chore &>(choreConst);
        std::cout << "  "
                  << (i <= 10 ? " " : "")
                  << (i <= 100 ? " " : "")
                  << ++i << "  " << chore.name();
        for (size_t j = chore.name().length(); j != 30; ++j)
            std::cout << ' ';
        std::cout << PROMPT;

        size_t estimate;
        std::cin >> estimate;
        std::getline(std::cin, dummy);
        chore.estimate(name, estimate);
    }

}

void Sprint::list(){
    size_t i = 0;
    for (const Chore &chore : _chores)
        std::cout << "  "
                  << (i < 10 ? " " : "")
                  << (i < 100 ? " " : "")
                  << ++i << "  " << chore.name() << std::endl;
}

void Sprint::remove(){
    list();
    std::cout << "Please enter the number of the chore to remove: " << PROMPT;
    size_t toRemove;
    std::cin >> toRemove;
    std::string dummy;
    std::getline(std::cin, dummy);

    if (toRemove == 0 || toRemove > _chores.size())
        std::cout << "Invalid number." << std::endl;
    else{
        size_t i = 0;
        for (std::set<Chore>::iterator it = _chores.begin(); it != _chores.end(); ++it)
            if (++i == toRemove){
                _chores.erase(it);
                break;
            }
    }
}

void Sprint::backup(){

}
