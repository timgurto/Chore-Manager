// (C) 2016 Tim Gurto

#ifndef CHORE_H
#define CHORE_H

#include <map>
#include <string>

class Chore{
    std::string _name;
    std::string _owner; // If blank, a shared chore.
    std::map<std::string, size_t> _estimates; // 0 = unestimated
    std::list<std::string> _people;

public:
    Chore(const std::string &name, const std::string &owner):
        _name(name),
        _owner(owner){}

    const std::string &name() const { return _name; }
    size_t estimate(const std::string &name) const {
        auto it = _estimates.find(name);
        if (it == _estimates.end())
            return 0;
        return it->second;
    }

    void estimate(const std::string &name, size_t estimate){ _estimates[name] = estimate; }

    bool operator<(const Chore &rhs) const { return _name.compare(rhs._name) < 0; }
};

#endif
