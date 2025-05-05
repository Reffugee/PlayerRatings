#pragma once
#include "Rating.h"
#include "Player.h"

class ForwardRating : public Rating {
public:
    double calculateRating(const Player& p) const override;
};
