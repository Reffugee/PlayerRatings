// GoalkeeperRating.h
#pragma once
#include "Rating.h"
#include "Player.h"

class GoalkeeperRating : public Rating {
public:
    double calculateRating(const Player& p) const override;
};

