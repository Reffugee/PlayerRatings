// MidfielderRating.h
#pragma once
#include "Rating.h"
#include "Player.h"

class MidfielderRating : public Rating {
public:
    double calculateRating(const Player& p) const override;
};

