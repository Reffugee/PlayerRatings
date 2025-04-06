#include "RatingManager.h"

RatingManager::RatingManager() : ratingStrategy(nullptr) {}

void RatingManager::setRatingStrategy(Rating* strategy) {
    ratingStrategy = strategy;
}

double RatingManager::ratePlayer(const Player& p) {
    if (ratingStrategy)
        return ratingStrategy->calculateRating(p);
    return 0.0;
}
