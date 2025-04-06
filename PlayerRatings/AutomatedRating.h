#ifndef AUTOMATED_RATING_H
#define AUTOMATED_RATING_H

#include "Rating.h"

class AutomatedRating : public Rating {
public:
    double calculateRating(const Player& p) const override;
};

#endif // AUTOMATED_RATING_H