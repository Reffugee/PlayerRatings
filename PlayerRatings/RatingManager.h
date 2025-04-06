#ifndef RATING_MANAGER_H
#define RATING_MANAGER_H

#include "Player.h"
#include "Rating.h"

class RatingManager {
private:
    Rating* ratingStrategy;
public:
    RatingManager();
    void setRatingStrategy(Rating* strategy);
    double ratePlayer(const Player& p);
};

#endif // RATING_MANAGER_H