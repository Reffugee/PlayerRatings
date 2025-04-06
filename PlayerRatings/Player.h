#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Stats.h"
using namespace std;

class Player {
public:
    string name;
    string position;
    Stats stats;

    Player(const string& n, const string& pos, const Stats& s);
};

#endif // PLAYER_H
