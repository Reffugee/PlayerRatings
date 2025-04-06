#include "Stats.h"

Stats::Stats(int g, int a, int t, int kp, int sd,
    int bp, int f, int os, int yc, int rc,
    int d, int elg, int og)
    : goals(g), assists(a), tackles(t), keyPasses(kp), successfulDribbles(sd),
    badPasses(bp), fouls(f), offsides(os), yellowCards(yc), redCards(rc),
    dispossessions(d), errorsLeadingToGoal(elg), ownGoals(og) {
}
