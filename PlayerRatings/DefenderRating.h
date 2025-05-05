// DefenderRating.h
#pragma once
#include "Rating.h"
#include "Player.h"

class DefenderRating : public Rating {
public:
    double calculateRating(const Player& p) const override;
};

