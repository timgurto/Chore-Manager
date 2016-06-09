// (C) 2016 Tim Gurto

#ifndef CHORE_H
#define CHORE_H

#include <map>
#include <string>

class Chore{
    std::string _name;
    std::string _owner; // If blank, a shared chore.
    std::map<std::string, int> _estimates; // 0 = unestimated

public:
    Chore(const std::string &name, const std::string &owner):
        _name(name),
        _owner(owner){}

    const std::string &name() const { return _name; }
    int estimate(const std::string &name) { return _estimates[name]; }

    bool operator<(const Chore &rhs) const { return _name.compare(rhs._name) < 0; }
};

#endif
