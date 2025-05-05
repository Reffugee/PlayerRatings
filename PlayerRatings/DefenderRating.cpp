// DefenderRating.cpp
#include "DefenderRating.h"

double DefenderRating::calculateRating(const Player& p) const {
    double baseline = 6.0;

    const Stats& s = p.stats;

    double positives =
        s.goals * 1.0 +
        s.assists * 0.7 +
        s.tackles * 0.3 +
        s.keyPasses * 0.4 +
        s.successfulDribbles * 0.2 +
        s.finalthirdpass * 0.1 +
        s.progressivecarries * 0.1 +
        s.interceptions * 0.2 +
        s.clearances * 0.1 +
        s.aerialDuelsWon * 0.1 +
        s.blocks * 0.1;

    double negatives =
        s.badPasses * 0.2 +
        s.fouls * 0.5 +
        s.offsides * 0.1 +
        s.yellowCards * 0.8 +
        s.redCards * 1.5 +
        s.ownGoals * 1.0 +
        s.unsuccessfulDribbles * 0.3;
    
    double rating = baseline + positives - negatives;

    if (rating < 1.0) rating = 1.0;
    if (rating > 10.0) rating = 10.0;

    return rating;
}
