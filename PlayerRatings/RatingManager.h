﻿#ifndef RATINGMANAGER_H
#define RATINGMANAGER_H

#include <memory>
#include <map>
#include <string>
#include <ctime>
#include <stdexcept>

#include "Rating.h"
#include "ForwardRating.h"
#include "MidfielderRating.h"
#include "DefenderRating.h"
#include "GoalkeeperRating.h"
#include "Player.h"

class RatingManager {
public:
    RatingManager();

    void setRatingStrategy(std::unique_ptr<Rating> strategy);

    double ratePlayer(const Player& p);

    void saveRatingsToCSV(const std::string& filename) const;

    double getAverageRatingLastDays(const std::string& playerName, int lookbackDays);

    static int parseDateToDays(const std::string& dateStr);

    static int getTodayInDays();


private:
    std::unique_ptr<Rating> overrideStrat;

    ForwardRating    fwdStrat;
    MidfielderRating midStrat;
    DefenderRating   defStrat;
    GoalkeeperRating keepStrat;

    struct MatchRating {
        int    dateDays = 0;
        std::string position;
        double      autoScore;
        bool        hasUserScore;
        double      userScore;
        MatchRating()
            : position(), autoScore(0.0),
            hasUserScore(false), userScore(0.0) {
        }
    };

    std::map<std::string, std::map<std::string, MatchRating>> matchRatings;
};

#endif // RATINGMANAGER_H
