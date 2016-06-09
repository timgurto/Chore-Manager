// (C) 2016 Tim Gurto

#ifndef SPRINT_H
#define SPRINT_H

#include <list>
#include <string>

class Sprint{
    time_t _startTime, _endTime;
    bool _inProgress; //whether the sprint has begun (as opposed to still being set up)

public:
    // Opens the last sprint if possible.  If it is closed, or there is none, create a new sprint.
    Sprint();

    bool inProgress() const { return _inProgress; }

    void setup(); // Terminal managing a sprint's preparation.  May call resume().
    void resume(); // Terminal managing an in-progress sprint.

};

#endif
