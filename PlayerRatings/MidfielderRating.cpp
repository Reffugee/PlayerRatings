// MidfielderRating.cpp
#include "MidfielderRating.h"

double MidfielderRating::calculateRating(const Player& p) const {
    double baseline = 6.0;

    const Stats& s = p.stats;

    double positives =
        p.stats.goals * 1.0 +
        p.stats.assists * 0.8 +
        p.stats.tackles * 0.3 +
        p.stats.keyPasses * 0.5 +
        p.stats.successfulDribbles * 0.2 +
        p.stats.finalthirdpass * 0.2 +
        p.stats.progressivecarries * 0.3 +
        p.stats.interceptions * 0.1 +
        p.stats.blocks * 0.1;
    double negatives =
        p.stats.badPasses * 0.2 +
        p.stats.fouls * 0.3 +
        p.stats.offsides * 0.1 +
        p.stats.yellowCards * 0.7 +
        p.stats.redCards * 1.5 +
        p.stats.ownGoals * 1.0 +
        p.stats.unsuccessfulDribbles * 0.1;

    double rating = baseline + positives - negatives;

    if (rating < 1.0) rating = 1.0;
    if (rating > 10.0) rating = 10.0;

    return rating;
}
