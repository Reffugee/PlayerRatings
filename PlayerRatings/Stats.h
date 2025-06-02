#ifndef STATS_H
#define STATS_H

struct Stats {
    std::string matchDate;
    // positive
    int goals = 0;
    int assists = 0;
    int tackles = 0;
    int keyPasses = 0;
    int successfulDribbles = 0;
    int finalthirdpass = 0;
    int progressivecarries = 0;

    // negative
    int badPasses = 0;
    int fouls = 0;
    int offsides = 0;
    int yellowCards = 0;
    int redCards = 0;
    int ownGoals = 0;
    int unsuccessfulDribbles = 0;

    // defender-specific
    int interceptions = 0;
    int clearances = 0;
    int aerialDuelsWon = 0;
    int blocks = 0;

    // keeper-specific
    int saves = 0;
    int cleanSheets = 0;
    int crossesStopped = 0;
    int goalsConceded = 0;
    int punchesMissed = 0;
};

#endif // STATS_H
