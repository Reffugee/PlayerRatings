#include "PlayerLoader.h"
#include "AutomatedRating.h"
#include "UserRating.h"
#include "RatingManager.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <exception>
using namespace std;

int main() {
    RatingManager mgr;

    while (true) {
        vector<Player> players = loadPlayers("players.csv");
        if (players.empty()) {
            cerr << "No players found in players.csv\n";
            break;  
        }

        map<string, vector<const Player*>> grouped;
        for (const auto& p : players) {
            grouped[p.name].push_back(&p);
        }

        vector<string> uniqueNames;
        for (const auto& kv : grouped) {
            uniqueNames.push_back(kv.first);
        }

        cout << "--- Select a Player (or 0 to Exit) ---\n";
        for (size_t i = 0; i < uniqueNames.size(); ++i) {
            const string& name = uniqueNames[i];
            int count = grouped[name].size();
            cout << i + 1 << ". " << name;
            if (count > 1) cout << " (" << count << " entries)";
            cout << "\n";
        }
        cout << "0. Exit\n";

        int choice;
        cout << "Your choice: ";
        if (!(cin >> choice) || choice < 0 || choice >(int)uniqueNames.size()) {
            cout << "Invalid choice, try again.\n";
            continue;
        }
        if (choice == 0) {
            break;
        }

        string selectedName = uniqueNames[choice - 1];
        const auto& entries = grouped[selectedName];

        AutomatedRating autoR;
        while (true) {
            cout << "\n--- Actions for " << selectedName << " ---\n"
                << "1) Automated rating for all entries\n"
                << "2) Manual rating for a specific match\n"
                << "3) Back to player selection\n"
                << "Choice: ";
            if (!(cin >> choice)) break;

            if (choice == 1) {
                mgr.setRatingStrategy(nullptr);
                cout << "Auto scores for '" << selectedName << "':\n";
                for (size_t i = 0; i < entries.size(); ++i) {
                    double score = mgr.ratePlayer(*entries[i]);
                    cout << "  " << i + 1 << ") vs. " << entries[i]->opponent << " → " << score << "\n";
                }
                mgr.saveRatingsToCSV("ratings.csv");

                cout << "\nView match stats? (y/n): "; char ans; cin >> ans;
                if (ans == 'y' || ans == 'Y') {
                    cout << "Select match to view stats:\n";
                    for (size_t i = 0; i < entries.size(); ++i)
                        cout << "  " << i + 1 << ") vs. " << entries[i]->opponent << "\n";
                    int m; cout << "Choice: ";
                    if (cin >> m && m >= 1 && m <= (int)entries.size()) {
                        const Player* p = entries[m - 1];
                        const Stats& s = p->stats;
                        cout << "\nStats for " << p->name << " (" << p->position << ") vs. " << p->opponent << ":\n"
                            << "  Goals:               " << s.goals << "\n"
                            << "  Assists:             " << s.assists << "\n"
                            << "  Tackles:             " << s.tackles << "\n"
                            << "  Key Passes:          " << s.keyPasses << "\n"
                            << "  Successful Dribbles: " << s.successfulDribbles << "\n"
                            << "  Final Third Passes:  " << s.finalthirdpass << "\n"
                            << "  Progressive Carries: " << s.progressivecarries << "\n"
                            << "  Bad Passes:          " << s.badPasses << "\n"
                            << "  Fouls:               " << s.fouls << "\n"
                            << "  Offsides:            " << s.offsides << "\n"
                            << "  Yellow Cards:        " << s.yellowCards << "\n"
                            << "  Red Cards:           " << s.redCards << "\n"
                            << "  Own Goals:           " << s.ownGoals << "\n"
                            << "  Unsuccessful Dribbles:" << s.unsuccessfulDribbles << "\n"
                            << "  Interceptions:       " << s.interceptions << "\n"
                            << "  Clearances:          " << s.clearances << "\n"
                            << "  Aerial Duels Won:    " << s.aerialDuelsWon << "\n"
                            << "  Blocks:              " << s.blocks << "\n"
                            << "  Saves:               " << s.saves << "\n"
                            << "  Clean Sheets:        " << s.cleanSheets << "\n"
                            << "  Crosses Stopped:     " << s.crossesStopped << "\n"
                            << "  Goals Conceded:      " << s.goalsConceded << "\n"
                            << "  Punches Missed:      " << s.punchesMissed << "\n";
                    }
                    else {
                        cout << "Invalid match number.\n";
                    }
                }
            }
            else if (choice == 2) {
                cout << "Select match to rate:\n";
                for (size_t i = 0; i < entries.size(); ++i)
                    cout << "  " << i + 1 << ") vs. " << entries[i]->opponent << "\n";
                int m; cout << "Choice: ";
                if (cin >> m && m >= 1 && m <= (int)entries.size()) {
                    double score;
                    cout << "Enter your score for match vs. " << entries[m - 1]->opponent << " (1 to 10): ";
                    cin >> score;
                    mgr.setRatingStrategy(make_unique<UserRating>(score));
                    double s = mgr.ratePlayer(*entries[m - 1]);
                    cout << "Recorded user score: " << s << "\n";
                    mgr.saveRatingsToCSV("ratings.csv");
                    mgr.setRatingStrategy(nullptr);
                }
                else {
                    cout << "Invalid match number.\n";
                }
            }
            else if (choice == 3) {
                break;
            }
            else {
                cout << "Invalid choice, try again.\n";
            }
        }
    }
        try {
            mgr.saveRatingsToCSV("ratings.csv");
            cout << "All ratings saved to ratings.csv\n";
        }
        catch (const exception& ex) {
            cerr << "Error saving ratings: " << ex.what() << "\n";
        }

    return 0;
}