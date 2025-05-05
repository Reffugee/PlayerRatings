// GoalkeeperRating.cpp
#include "GoalkeeperRating.h"

double GoalkeeperRating::calculateRating(const Player& p) const {
    double baseline = 6;

    const Stats& s = p.stats;

    double positives =
        0.3 * p.stats.saves
        + 0.5 * p.stats.cleanSheets
        + 0.2 * p.stats.crossesStopped;

    double negatives =
        0.5 * p.stats.goalsConceded
        + 0.5 * p.stats.punchesMissed
        + 0.3 * p.stats.yellowCards
        + 1 * p.stats.redCards;
    
    double rating = baseline + positives - negatives;

    if (rating < 1.0) rating = 1.0;
    if (rating > 10.0) rating = 10.0;

    return rating;
}
