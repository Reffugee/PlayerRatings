#ifndef RATING_H
#define RATING_H

#include "Player.h"

class Rating {
public:
    virtual double calculateRating(const Player& p) const = 0;
    virtual ~Rating() {} // Virtual destructor
};

#endif // RATING_H
