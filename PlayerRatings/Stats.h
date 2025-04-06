#ifndef STATS_H
#define STATS_H

class Stats {
public:
    // Positive stats
    int goals;
    int assists;
    int tackles;
    int keyPasses;
    int successfulDribbles;

    // Negative stats
    int badPasses;
    int fouls;
    int offsides;
    int yellowCards;
    int redCards;
    int dispossessions;
    int errorsLeadingToGoal;
    int ownGoals;

    Stats(int g = 0, int a = 0, int t = 0, int kp = 0, int sd = 0,
        int bp = 0, int f = 0, int os = 0, int yc = 0, int rc = 0,
        int d = 0, int elg = 0, int og = 0);
};

#endif
