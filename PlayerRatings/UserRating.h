#ifndef USER_RATING_H
#define USER_RATING_H

#include "Rating.h"

class UserRating : public Rating {
private:
    double userScore;
public:
    UserRating(double score);
    double calculateRating(const Player& p) const override;
};

#endif // USER_RATING_H