#include "AutomatedRating.h"

double AutomatedRating::calculateRating(const Player& p) const {
    double baseline = 6.0;

    double positives =
        p.stats.goals * 1.0 +
        p.stats.assists * 0.7 +
        p.stats.tackles * 0.3 +
        p.stats.keyPasses * 0.4 +
        p.stats.successfulDribbles * 0.3;

    double negatives =
        p.stats.badPasses * 0.2 +
        p.stats.fouls * 0.4 +
        p.stats.offsides * 0.1 +
        p.stats.yellowCards * 0.7 +
        p.stats.redCards * 1.5 +
        p.stats.dispossessions * 0.4 +
        p.stats.errorsLeadingToGoal * 0.7 +
        p.stats.ownGoals * 1.0;

    double rating = baseline + positives - negatives;

    // Safety clamp
    if (rating < 1.0) rating = 1.0;
    if (rating > 10.0) rating = 10.0;

    return rating;
}

