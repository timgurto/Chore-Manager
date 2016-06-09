// (C) 2016 Tim Gurto

#ifndef SPRINT_H
#define SPRINT_H

#include <vector>
#include <set>
#include <string>

#include "Chore.h"
#include "Task.h"

class Sprint{
    time_t _startTime, _endTime;
    bool _inProgress; //whether the sprint has begun (as opposed to still being set up)
    std::vector<std::string> _people;
    std::set<Chore> _chores; // Before sprint
    std::set<Task> _tasks; // during sprint

public:
    // Opens the last sprint if possible.  If it is closed, or there is none, create a new sprint.
    Sprint();

    bool inProgress() const { return _inProgress; }

    void setup(); // Terminal managing a sprint's preparation.  May call resume().
    void resume(); // Terminal managing an in-progress sprint.

    void addChore(); // Add a new chore
    void estimate(); // Estimate one or all chores
    void list(); // List all current chores, and who has estimated them
    void remove(); // Remove a chore
    void backup(); // Back up sprint info

    bool checkEstimates(); // Make sure all estimates are in
    void allocate();

};

#endif
