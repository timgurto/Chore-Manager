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

}

void Sprint::unfinishTask(){

}
