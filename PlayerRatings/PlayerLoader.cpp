#include "PlayerLoader.h"
#include "Stats.h"

#include <fstream>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iostream>

using namespace std;

vector<Player> loadPlayers(const string& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error("Cannot open " + filename);

    string line;
    if (!getline(in, line))
        throw runtime_error("Empty file");

    vector<string> headers;
    {
        istringstream hss(line);
        string col;
        while (getline(hss, col, ',')) {
            col.erase(0, col.find_first_not_of(" \t"));
            col.erase(col.find_last_not_of(" \t") + 1);
            headers.push_back(col);
        }
    }


    if (headers.size() < 4)
        throw runtime_error("Need header: name,position,opponent,<at least one stat>");

    const size_t STAT_START = 3;  // index 0=name,1=position,2=opponent

    static const unordered_map<string, int Stats::*> statMap = {
        {"goals",&Stats::goals}, {"assists",&Stats::assists},
        {"tackles",&Stats::tackles}, {"keyPasses",&Stats::keyPasses},
        {"successfulDribbles",&Stats::successfulDribbles},
        {"finalthirdpass",&Stats::finalthirdpass},
        {"progressivecarries",&Stats::progressivecarries},
        {"badPasses",&Stats::badPasses}, {"fouls",&Stats::fouls},
        {"offsides",&Stats::offsides}, {"yellowCards",&Stats::yellowCards},
        {"redCards",&Stats::redCards}, {"ownGoals",&Stats::ownGoals},
        {"unsuccessfulDribbles",&Stats::unsuccessfulDribbles},
        {"interceptions",&Stats::interceptions},
        {"clearances",&Stats::clearances}, {"aerialDuelsWon",&Stats::aerialDuelsWon},
        {"blocks",&Stats::blocks}, {"saves",&Stats::saves},
        {"cleanSheets",&Stats::cleanSheets}, {"crossesStopped",&Stats::crossesStopped},
        {"goalsConceded",&Stats::goalsConceded}, {"punchesMissed",&Stats::punchesMissed}
    };

    vector<int Stats::*> colMember(headers.size(), nullptr);
    for (size_t i = STAT_START; i < headers.size(); ++i) {
        auto it = statMap.find(headers[i]);
        if (it != statMap.end()) {
            colMember[i] = it->second;
        } else {
            cerr << "Warning: unknown stat column ‘" << headers[i] << "’—ignored\n";
        }
    }

    vector<Player> out;
    while (getline(in, line)) {
        if (line.empty()) continue;
        istringstream lss(line);
        vector<string> cells;
        string cell;
        while (getline(lss, cell, ',')) {
            cell.erase(0, cell.find_first_not_of(" \t"));
            cell.erase(cell.find_last_not_of(" \t") + 1);
            cells.push_back(cell);
        }
        if (cells.size() != headers.size())
            throw runtime_error("Bad row in CSV (wrong number of columns)");

        const string& name = cells[0];
        const string& pos  = cells[1];
        const string& opp  = cells[2];

        Stats S;
        for (size_t i = STAT_START; i < cells.size(); ++i) {
            if (colMember[i]) {
                S.*(colMember[i]) = stoi(cells[i]);
            }
        }

        out.emplace_back(name, pos, opp, S);
    }

    return out;
}

