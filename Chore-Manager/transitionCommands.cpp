// (C) 2016 Tim Gurto

#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "Sprint.h"
#include "Task.h"
#include "constants.h"

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

// Some metric.  zero = completely fair.
double unfairness(std::vector<double> v){
    // Variance
    double mean = 0;
    for (double d : v)
        mean += d;
    mean = mean / v.size();
    double variance = 0;
    for (double d : v)
        variance +=  (d - mean) * (d - mean);
    return variance;
}

void Sprint::allocate(){
    std::cout << std::endl << "Allocating tasks..." << std::endl;

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
        if (originalChore.owner() != "")
            continue;
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

    std::cout << "Unfairness=" << unfairness(totals) << std::endl;

    while (true) {

        // Try giving smallest-max item from biggest person to smallest
        size_t biggestPerson = 0;
        for (size_t i = 1; i != _people.size(); ++i)
            if (totals[i] > totals[biggestPerson])
                biggestPerson = i;
        std::vector<double> unfairnessTest(_people.size());
        unfairnessTest[biggestPerson] = unfairness(totals);
        Chore toReallocate = stacks[biggestPerson].top();
        totals[biggestPerson] -= toReallocate.minEstimate;

        // Test fairness with each other person
        for (size_t i = 0; i != _people.size(); ++i){
            if (i == biggestPerson)
                continue;
            double estimate = modifiers[i] * toReallocate.estimate(_people[i]);
            totals[i] += estimate;
            unfairnessTest[i] = unfairness(totals);
            totals[i] -= estimate;
        }
        totals[biggestPerson] += toReallocate.minEstimate;

        // Check which is the most fair
        size_t target = 0;
        for (size_t i = 1; i != _people.size(); ++i)
            if (unfairnessTest[i] < unfairnessTest[target])
                target = i;
        std::cout << "Unfairness=" << unfairnessTest[target] << std::endl;

        if (target == biggestPerson)
            break; // We're done allocating.

        totals[biggestPerson] -= toReallocate.minEstimate;
        totals[target] += modifiers[target] * toReallocate.estimate(_people[target]);
        stacks[biggestPerson].pop();
        stacks[target].push(toReallocate);
    }

    // Convert chores to tasks
    for (size_t i = 0; i != _people.size(); ++i)
        while (!stacks[i].empty()){
            const Chore &chore = stacks[i].top();
            Task task;
            task.name = chore.name();
            task.assignee = i;
            task.effort = modifiers[i] * chore.estimate(_people[i]);
            task.isDone = false;
            _tasks.insert(task);
            effortRemaining[i] += task.effort;
            stacks[i].pop();
        }

    // TODO fix this by making Chore::_owner an index
    std::map<std::string, size_t> nameLookup;
    for (size_t i = 0; i != _people.size(); ++i)
        nameLookup[_people[i]] = i;

    // Also include non-shared chores
    std::vector<double> extra(_people.size(), 0);
    for (const Chore &chore : _chores){
        if (chore.owner() == "")
            continue;
        Task task;
        task.name = chore.name();
        task.assignee = nameLookup[chore.owner()];
        task.effort = modifiers[task.assignee] * chore.estimate(chore.owner());
        task.isDone = false;
        _tasks.insert(task);
        effortRemaining[task.assignee] += task.effort;
        extra[task.assignee] += task.effort;
    }

    std::cout << "Tasks have been allocated." << std::endl << std::endl;
    std::streamsize oldPrecision = std::cout.precision();
    for (size_t i = 0; i != _people.size(); ++i){
        std::cout.precision(4);
        std::cout << _people[i] << std::endl
                  << "             Shared chores: " << std::setw(4) << totals[i]
                  << std::endl
                  << "           Personal chores: " << std::setw(4) << extra[i]
                  << std::endl
                  << "                     TOTAL: " << std::setw(4) << (totals[i] + extra[i])
                  << std::endl;
        std::cout.precision(oldPrecision);

        size_t num = 0;
        for (const Task &task : _tasks)
            if (task.assignee == i){
                std::cout << std::setw(3) << ++num << "  " << task.name;
                for (size_t i = task.name.size(); i < 30; ++i)
                    std::cout << ' ';
                
                std::cout.precision(4);
                std::cout << std::setw(4) << task.effort << std::endl;
                std::cout.precision(oldPrecision);
            }
        std::cout << std::endl;
    }
}

void Sprint::getEndTime(){
    time_t endTime;
    while (true) {
        size_t year, month, day, hour;

        std::cout << "When will this sprint end?" << std::endl;
        std::cout << "        Year: " << PROMPT; std::cin >> year;
        std::cout << "Month (1-12): " << PROMPT; std::cin >> month;
        std::cout << "         Day: " << PROMPT; std::cin >> day;
        std::cout << " Hour (0-23): " << PROMPT; std::cin >> hour;
        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        time(&endTime);
        tm *timeInfo = localtime(&endTime);
        timeInfo->tm_year = year - 1900;
        timeInfo->tm_mon = month - 1;
        timeInfo->tm_mday = day;
        timeInfo->tm_hour = hour;
        timeInfo->tm_min = timeInfo->tm_sec = 0;
        endTime = mktime(timeInfo);

        std::cout << "The sprint will end at " << asctime(timeInfo)
                  << "Is this correct? (y/n) " << PROMPT;
        std::string response;
        std::getline(std::cin, response);
        if (response == "y")
            break;
    }

    std::ofstream endFile("sprint/end.dat");
    endFile << endTime << std::endl;;
}

void Sprint::recordStartTime(){
    std::ofstream startFile("sprint/start.dat");
    startFile << time(0) << std::endl;;

    {
        std::ofstream graphData;
        graphData.open("sprint/burndown.dat", std::ios_base::app);
        graphData << time(0);
        for (size_t i = 0; i != _people.size(); ++i)
            graphData << ',' << effortRemaining[i];
        graphData << std::endl;
    }
    system("burndown.bat");
}
