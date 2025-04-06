#include "UserRating.h"

UserRating::UserRating(double score) : userScore(score) {}

double UserRating::calculateRating(const Player& p) const {

    return userScore;
}
