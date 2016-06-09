// (C) 2016 Tim Gurto

#include <iostream>
#include <string>

#include "Sprint.h"

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

}

void Sprint::backup(){

}
