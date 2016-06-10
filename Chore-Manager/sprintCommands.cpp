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

void clearCin2(){
    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Sprint::listTasks(bool includeFinished){

    size_t i = 0;
    for (const Task &task : _tasks) {
        if (task.isDone && !includeFinished)
            continue;

        // Number
        std::cout << "  "
                  << (++i < 10 ? " " : "")
                  << (i < 100 ? " " : "")
                  << i << "  " << task.name;

        // Name
        for (size_t j = task.name.length(); j < 30; ++j)
            std::cout << ' ';

        // Effort
        std::streamsize oldPrecision = std::cout.precision();
        std::cout.precision(4);
        std::cout << std::setw(4) << task.effort
                  << ' ' << (task.isDone ? 'X' : ' ') << ' '
                  << _people[task.assignee] << std::endl;
    }
}

void Sprint::finishTask(){
    size_t i = 0;
    for (const Task &task : _tasks) {
        if (task.isDone)
            continue;

        std::cout << "  "
                  << (++i < 10 ? " " : "")
                  << (i < 100 ? " " : "")
                  << i << "  " << task.name << std::endl;
    }
        
    std::cout << "Please enter the number of the task to finish: " << PROMPT;
    size_t num;
    std::cin >> num;
    clearCin2();

    if (num == 0 || num > i) {
        std::cout << "Invalid number" << std::endl;
        return;
    }

    i = 0;
    for (const Task &taskConst : _tasks) {
        Task &task = const_cast<Task &>(taskConst);
        if (task.isDone)
            continue;
        ++i;
        if (i == num){
            task.isDone = true;

            std::ofstream graphData;
            graphData.open("sprint/burndown.dat", std::ios_base::app);

            graphData << time(0);
            for (size_t i = 0; i != _people.size(); ++i)
                graphData << ',' << effortRemaining[i];
            graphData << std::endl;

            effortRemaining[task.assignee] -= task.effort;

            graphData << time(0);
            for (size_t i = 0; i != _people.size(); ++i)
                graphData << ',' << effortRemaining[i];
            graphData << std::endl;
            break;
        }
    }
    system("burndown.bat");
}

void Sprint::unfinishTask(){
    std::cout << "Coming soon!" << std::endl;
}

void Sprint::refreshChart(){
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
