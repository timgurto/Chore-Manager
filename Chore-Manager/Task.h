// (C) 2016 Tim Gurto

#ifndef TASK_H
#define TASK_H

#include <string>

struct Task{
    std::string _name;
    size_t _assignee;
    double _effort;
    bool _isDone;

    bool operator<(const Task &rhs) const{ return _name.compare(rhs._name) < 0; }
};

#endif
