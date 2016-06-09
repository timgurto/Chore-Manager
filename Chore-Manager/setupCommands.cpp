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

}

void Sprint::list(){
    for (const Chore &chore : _chores)
        std::cout << "  " << chore.name() << std::endl;
}

void Sprint::remove(){
    size_t i = 0;
    for (const Chore &chore : _chores)
        std::cout << "  "
                  << (i <= 10 ? " " : "")
                  << (i <= 100 ? " " : "")
                  << ++i << "  " << chore.name() << std::endl;
    std::cout << "Please enter the number of the chore to remove: " << PROMPT;
    size_t toRemove;
    std::cin >> toRemove;
    std::string dummy;
    std::getline(std::cin, dummy);

    if (toRemove == 0 || toRemove > _chores.size())
        std::cout << "Invalid number." << std::endl;
    else{
        i = 0;
        for (std::set<Chore>::iterator it = _chores.begin(); it != _chores.end(); ++it)
            if (++i == toRemove){
                _chores.erase(it);
                break;
            }
    }
}

void Sprint::backup(){

}
