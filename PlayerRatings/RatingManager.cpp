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
double RatingManager::getAverageRatingLastDays(
    const std::string& playerName,
    int lookbackDays
) {
    auto it = matchRatings.find(playerName);
    if (it == matchRatings.end()) {
        // No matches recorded for this player
        return 0.0;
    }

    int todayDays = getTodayInDays();
    double sum = 0.0;
    int count = 0;

    // it->second is a std::map<std::string, MatchRating>
    for (const auto& pair : it->second) {
        // pair.first  = opponent (string)
        // pair.second = MatchRating
        const MatchRating& mr = pair.second;

        // Pick the correct score:
        double score = mr.hasUserScore ? mr.userScore : mr.autoScore;

        int ageDays = todayDays - mr.dateDays;
        if (ageDays < 0) {
            // Future‐dated match? Skip.
            continue;
        }
        if (lookbackDays > 0 && ageDays >= lookbackDays) {
            // Outside the requested window → skip
            continue;
        }

        // Otherwise, include this match in the average
        sum += score;
        ++count;
    }

    if (count == 0) {
        return 0.0;
    }
    return (sum / static_cast<double>(count));
}

int RatingManager::parseDateToDays(const std::string& dateStr) {
    if (dateStr.size() != 10 || dateStr[4] != '-' || dateStr[7] != '-') {
        throw std::invalid_argument("Bad date format: " + dateStr);
    }
    int year = std::stoi(dateStr.substr(0, 4));
    int month = std::stoi(dateStr.substr(5, 2));
    int day = std::stoi(dateStr.substr(8, 2));

    std::tm tm = {};
    tm.tm_year = year - 1900;    // years since 1900
    tm.tm_mon = month - 1;      // months [0..11]
    tm.tm_mday = day;            // day [1..31]
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;            // let mktime figure DST

    std::time_t t = std::mktime(&tm);
    if (t == static_cast<std::time_t>(-1)) {
        throw std::runtime_error("mktime failed for date: " + dateStr);
    }
    return static_cast<int>(t / 86400);
}

int RatingManager::getTodayInDays() {
    std::time_t now = std::time(nullptr);
    return static_cast<int>(now / 86400);
}

double RatingManager::ratePlayer(const Player& p) {
    // 1) pick strategy
    Rating* strat = overrideStrat.get();
    if (!strat) {
        std::string pos = p.position;
        std::transform(pos.begin(), pos.end(), pos.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (pos == "forward")        strat = &fwdStrat;
        else if (pos == "midfielder") strat = &midStrat;
        else if (pos == "defender")   strat = &defStrat;
        else if (pos == "goalkeeper") strat = &keepStrat;
        else                          strat = &fwdStrat;  // fallback
    }

    // 2) calculate the score
    double score = strat->calculateRating(p);

    // 3) record into per-match map
    MatchRating& mr = matchRatings[p.name][p.opponent];

    mr.dateDays = parseDateToDays(p.stats.matchDate);
    mr.position = p.position;

    if (overrideStrat) {
        // This is a user rating
        mr.hasUserScore = true;
        mr.userScore = score;
    }
    else {
        // Automated rating—overwrite autoScore every time
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
