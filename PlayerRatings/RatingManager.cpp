#include "RatingManager.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>

RatingManager::RatingManager()
    : overrideStrat(nullptr)
{
}

void RatingManager::setRatingStrategy(std::unique_ptr<Rating> strat) {
    overrideStrat = std::move(strat);
}

double RatingManager::ratePlayer(const Player& p) {
    // 1) pick strategy
    Rating* strat = overrideStrat.get();
    if (!strat) {
        std::string pos = p.position;
        std::transform(pos.begin(), pos.end(), pos.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (pos == "forward")    strat = &fwdStrat;
        else if (pos == "midfielder") strat = &midStrat;
        else if (pos == "defender")   strat = &defStrat;
        else if (pos == "goalkeeper") strat = &keepStrat;
        else                           strat = &fwdStrat;  // fallback
    }

    // 2) calculate
    double score = strat->calculateRating(p);

    // 3) record into per-match map
    MatchRating& mr = matchRatings[p.name][p.opponent];
    mr.position = p.position;

    if (overrideStrat) {
        // This is a user rating
        mr.hasUserScore = true;
        mr.userScore = score;
    }
    else {
        // This is the automated rating — only set it if it hasn't been rated before
        mr.autoScore = score;
    }

    return score;
}

void RatingManager::saveRatingsToCSV(const std::string& file) const {
    std::ofstream out(file.c_str());
    if (!out) throw std::runtime_error("Cannot open " + file);

    out << "player,position,opponent,automatedRating,userRating\n";
    for (auto playerIt = matchRatings.begin(); playerIt != matchRatings.end(); ++playerIt) {
        const std::string& player = playerIt->first;
        const auto& oppMap = playerIt->second;
        for (auto oppIt = oppMap.begin(); oppIt != oppMap.end(); ++oppIt) {
            const std::string& opponent = oppIt->first;
            const MatchRating& mr = oppIt->second;

            out << player << ','
                << mr.position << ','
                << opponent << ','
                << mr.autoScore << ','
                << (mr.hasUserScore
                    ? std::to_string(mr.userScore)
                    : std::string("N/A"))
                << '\n';
        }
    }
}
