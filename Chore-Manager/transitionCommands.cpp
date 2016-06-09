// (C) 2016 Tim Gurto

#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

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

double rms(std::vector<double> v){
    double sumSqr = 0;
    for (double d : v)
        sumSqr += d * d;
    double res = sqrt(sumSqr / v.size());
    std::cout << "RMS=" << res << std::endl;
    return res;
}

void Sprint::allocate(){
    // Calculate each person's modifier.  All estimates will be multiplied by this number.
    std::vector<double> modifiers(_people.size());
    for (size_t i = 0; i != _people.size(); ++i) {
        size_t sum = 0;
        for (const Chore &chore : _chores)
            if (chore.owner() == "")
                sum += chore.estimate(_people[i]);
        modifiers[i] = 100.0 / sum;
    }

    struct highestMax {
        bool operator() (const Chore &lhs, const Chore &rhs) const{
            if (lhs.maxEstimate != rhs.maxEstimate)
                return lhs.maxEstimate > rhs.maxEstimate;
            return lhs.minEstimate > rhs.minEstimate;
        }
    };

    std::multiset<Chore, highestMax> sortedChores;
    for (const Chore &originalChore : _chores){
        Chore chore = originalChore;
        chore.minEstimate = chore.maxEstimate = modifiers[0] * chore.estimate(_people[0]);

        for (size_t i = 1; i != _people.size(); ++i){
            double estimate = modifiers[i] * chore.estimate(_people[i]);
            if (estimate < chore.minEstimate)
                chore.minEstimate = estimate;
            if (estimate > chore.maxEstimate)
                chore.maxEstimate = estimate;
        }

        sortedChores.insert(chore);
    }

    // Create a stack for each person, and do an initial naive allocation based on preferences.
    std::vector<std::stack<Chore> > stacks(_people.size());
    std::vector<double> totals(_people.size(), 0);
    for (const Chore &chore : sortedChores){
        size_t prefPerson = 0;
        double min = modifiers[0] * chore.estimate(_people[0]);
        for (size_t i = 1; i != _people.size(); ++i) {
            double estimate = modifiers[i] * chore.estimate(_people[1]);
            if (estimate < min){
                prefPerson = i;
                min = estimate;
            }
        }
        stacks[prefPerson].push(chore);
        totals[prefPerson] += min;
    }

    // Calculate fairness (root mean square)
    rms(totals);
}
