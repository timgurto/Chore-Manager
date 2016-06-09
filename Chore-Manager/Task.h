// (C) 2016 Tim Gurto

#ifndef TASK_H
#define TASK_H

#include <string>

struct Task{
    std::string name;
    size_t assignee;
    double effort;
    bool isDone;

    bool operator<(const Task &rhs) const{ return name.compare(rhs.name) < 0; }
};

#endif
