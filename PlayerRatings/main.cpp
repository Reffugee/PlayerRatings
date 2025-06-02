#include "PlayerLoader.h"
#include "AutomatedRating.h"
#include "UserRating.h"
#include "RatingManager.h"

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>
#include <exception>
#include <iomanip>
#include <cstdlib>   // for system()
#include <limits>    // for numeric_limits
#include <algorithm> // for transform
#include <cmath>     // for ceil

using namespace std;

// Helper to clear the console (cross-platform)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Helper to pause until the user presses Enter
void pauseForUser() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Helper to search player names (case-sensitive substring match)
vector<string> searchPlayers(const set<string>& names, const string& keyword) {
    vector<string> results;
    for (const string& name : names) {
        if (name.find(keyword) != string::npos) {
            results.push_back(name);
        }
    }
    return results;
}

// Paginated selection for a list of strings; returns selected index (0-based) or -1 to cancel.
int paginateSelectStrings(const vector<string>& items) {
    if (items.empty()) return -1;
    const int pageSize = 5;
    int totalPages = static_cast<int>(ceil(items.size() / static_cast<double>(pageSize)));
    int currentPage = 0;

    while (true) {
        clearScreen();
        int start = currentPage * pageSize;
        int end = min(start + pageSize, static_cast<int>(items.size()));

        cout << "\nShowing items " << start + 1 << " to " << end
            << " of " << items.size() << "\n\n";
        for (int i = start; i < end; ++i) {
            cout << setw(4) << (i - start + 1) << "| " << items[i] << "\n";
        }
        cout << "\nPage " << (currentPage + 1) << " of " << totalPages << "\n";
        cout << "Enter number (1-" << (end - start) << ") to select, 'n' for next page, 'p' for prev page, or '0' to cancel: ";

        string input;
        cin >> input;
        if (input == "n" || input == "N") {
            if (currentPage < totalPages - 1) currentPage++;
        }
        else if (input == "p" || input == "P") {
            if (currentPage > 0) currentPage--;
        }
        else {
            // Try numeric
            int choice;
            try {
                choice = stoi(input);
            }
            catch (...) {
                continue;
            }
            if (choice == 0) return -1;
            if (choice >= 1 && choice <= (end - start)) {
                return start + (choice - 1);
            }
        }
    }
}

// Paginated selection for a list of Player* based on opponent; returns selected index (0-based) or -1 to cancel.
int paginateSelectMatches(const vector<const Player*>& entries) {
    if (entries.empty()) return -1;
    const int pageSize = 5;
    int totalPages = static_cast<int>(ceil(entries.size() / static_cast<double>(pageSize)));
    int currentPage = 0;

    while (true) {
        clearScreen();
        int start = currentPage * pageSize;
        int end = min(start + pageSize, static_cast<int>(entries.size()));

        cout << "\nShowing matches " << start + 1 << " to " << end
            << " of " << entries.size() << "\n\n";
        for (int i = start; i < end; ++i) {
            cout << setw(4) << (i - start + 1)
                << "| vs. " << entries[i]->opponent << "\n";
        }
        cout << "\nPage " << (currentPage + 1) << " of " << totalPages << "\n";
        cout << "Enter number (1-" << (end - start) << ") to select, 'n' for next page, 'p' for prev page, or '0' to cancel: ";

        string input;
        cin >> input;
        if (input == "n" || input == "N") {
            if (currentPage < totalPages - 1) currentPage++;
        }
        else if (input == "p" || input == "P") {
            if (currentPage > 0) currentPage--;
        }
        else {
            int choice;
            try {
                choice = stoi(input);
            }
            catch (...) {
                continue;
            }
            if (choice == 0) return -1;
            if (choice >= 1 && choice <= (end - start)) {
                return start + (choice - 1);
            }
        }
    }
}

// Paginated display for automated ratings (no selection): shows 5 per page with navigation
void paginateShowRatings(const vector<const Player*>& entries, RatingManager& mgr) {
    if (entries.empty()) {
        cout << "No match entries to display.\n";
        return;
    }

    const int pageSize = 5;
    int totalPages = static_cast<int>(ceil(entries.size() / static_cast<double>(pageSize)));
    int currentPage = 0;

    while (true) {
        clearScreen();

        int startIndex = currentPage * pageSize;
        int endIndex = min(startIndex + pageSize, static_cast<int>(entries.size()));

        // Header
        cout << "============================================================\n";
        cout << "      Auto-generated Ratings for Matches "
            << (startIndex + 1) << " - " << endIndex
            << " of " << entries.size() << "\n";
        cout << "============================================================\n\n";

        // Column titles
        cout << left << setw(5) << "#"
            << setw(25) << "Opponent"
            << setw(10) << "Rating" << "\n";
        cout << "------------------------------------------------------------\n";

        // Display ratings for the current page
        for (int i = startIndex; i < endIndex; ++i) {
            double score = mgr.ratePlayer(*entries[i]);
            cout << left << setw(5) << (i - startIndex + 1)
                << setw(25) << entries[i]->opponent
                << fixed << setprecision(2) << score << "\n";
        }

        // Navigation footer
        cout << "\nPage " << (currentPage + 1) << " of " << totalPages << "\n";
        cout << "Enter a number (1-" << (endIndex - startIndex) << ") to view details, "
            << "'n' for next page, 'p' for previous page, or '0' to return: ";

        string input;
        cin >> input;

        if (input == "n" || input == "N") {
            if (currentPage < totalPages - 1) {
                currentPage++;
            }
        }
        else if (input == "p" || input == "P") {
            if (currentPage > 0) {
                currentPage--;
            }
        }
        else {
            int choice;
            try {
                choice = stoi(input);
            }
            catch (...) {
                // Invalid input: refresh the display
                continue;
            }

            if (choice == 0) {
                // Exit pagination
                break;
            }

            // If the choice is within the valid range, show match details
            if (choice >= 1 && choice <= (endIndex - startIndex)) {
                const Player* p = entries[startIndex + (choice - 1)];
                clearScreen();

                // Detailed match statistics header
                cout << "============================================================\n";
                cout << "            Detailed Statistics for Match vs. "
                    << p->opponent << "\n";
                cout << "============================================================\n\n";

                // Basic information
                cout << left << setw(20) << "Player Name:" << p->name << "\n";
                cout << left << setw(20) << "Position:" << p->position << "\n";
                cout << left << setw(20) << "Opponent:" << p->opponent << "\n";
                cout << left << setw(20) << "Match Date:" << p->stats.matchDate << "\n\n";

                // All statistical fields
                cout << left << setw(25) << "Goals:" << p->stats.goals << "\n";
                cout << left << setw(25) << "Assists:" << p->stats.assists << "\n";
                cout << left << setw(25) << "Tackles:" << p->stats.tackles << "\n";
                cout << left << setw(25) << "Key Passes:" << p->stats.keyPasses << "\n";
                cout << left << setw(25) << "Successful Dribbles:" << p->stats.successfulDribbles << "\n";
                cout << left << setw(25) << "Final Third Passes:" << p->stats.finalthirdpass << "\n";
                cout << left << setw(25) << "Progressive Carries:" << p->stats.progressivecarries << "\n";
                cout << left << setw(25) << "Bad Passes:" << p->stats.badPasses << "\n";
                cout << left << setw(25) << "Fouls:" << p->stats.fouls << "\n";
                cout << left << setw(25) << "Offsides:" << p->stats.offsides << "\n";
                cout << left << setw(25) << "Yellow Cards:" << p->stats.yellowCards << "\n";
                cout << left << setw(25) << "Red Cards:" << p->stats.redCards << "\n";
                cout << left << setw(25) << "Own Goals:" << p->stats.ownGoals << "\n";
                cout << left << setw(25) << "Unsuccessful Dribbles:" << p->stats.unsuccessfulDribbles << "\n";
                cout << left << setw(25) << "Interceptions:" << p->stats.interceptions << "\n";
                cout << left << setw(25) << "Clearances:" << p->stats.clearances << "\n";
                cout << left << setw(25) << "Aerial Duels Won:" << p->stats.aerialDuelsWon << "\n";
                cout << left << setw(25) << "Blocks:" << p->stats.blocks << "\n";
                cout << left << setw(25) << "Saves:" << p->stats.saves << "\n";
                cout << left << setw(25) << "Clean Sheets:" << p->stats.cleanSheets << "\n";
                cout << left << setw(25) << "Crosses Stopped:" << p->stats.crossesStopped << "\n";
                cout << left << setw(25) << "Goals Conceded:" << p->stats.goalsConceded << "\n";
                cout << left << setw(25) << "Punches Missed:" << p->stats.punchesMissed << "\n\n";

                pauseForUser();
            }
        }
    }
}

int main() {
    RatingManager mgr;

    // 1) Load all players & populate matchRatings
    vector<Player> players;
    try {
        players = loadPlayers("players.csv");
        if (players.empty()) {
            cerr << "No players found in players.csv\n";
            return 1;
        }
        for (const auto& p : players) {
            mgr.ratePlayer(p);
        }
    }
    catch (const exception& ex) {
        cerr << "Error loading CSV: " << ex.what() << "\n";
        return 1;
    }

    // 2) Build a set of unique player names & grouping by name → vector<Player*>
    set<string> uniqueNames;
    map<string, vector<const Player*>> grouped;
    for (const auto& p : players) {
        uniqueNames.insert(p.name);
        grouped[p.name].push_back(&p);
    }

    // 3) Top‐level menu loop
    while (true) {
        clearScreen();
        cout << "====================================\n";
        cout << "          Player Rating Tool       \n";
        cout << "====================================\n";
        cout << "1) Per-Player Actions (automated/manual rating)\n";
        cout << "2) Calculate average ratings for last X days\n";
        cout << "3) Exit\n";
        cout << "------------------------------------\n";
        cout << "Your choice: ";

        int topChoice;
        if (!(cin >> topChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (topChoice) {
        case 1: {
            // ─── Per-Player Actions with search + pagination ───
            while (true) {
                clearScreen();
                cout << "\n--- Search or Select Player (or 0 to go back) ---\n";
                cout << "Enter a name (or part of one) to search: ";
                string searchTerm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, searchTerm);
                if (searchTerm == "0") {
                    break; // back to top‐level menu
                }

                vector<string> filtered = searchPlayers(uniqueNames, searchTerm);
                if (filtered.empty()) {
                    cout << "\nNo matches found.\n";
                    pauseForUser();
                    continue;
                }

                int selectedIdx = paginateSelectStrings(filtered);
                if (selectedIdx < 0) {
                    continue; // restart search
                }
                string selectedName = filtered[selectedIdx];
                const auto& entries = grouped[selectedName];

                // ─── Per‐player sub‐menu ───
                while (true) {
                    clearScreen();
                    cout << "\n--- Actions for " << selectedName << " ---\n";
                    cout << "1) Automated rating for all entries (paginated)\n";
                    cout << "2) Manual rating for a specific match\n";
                    cout << "3) Back to player selection\n";
                    cout << "Choice: ";

                    int choice;
                    if (!(cin >> choice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;  // invalid input → back to player selection
                    }

                    switch (choice) {
                    case 1: {
                        // Automated rating for all entries, paginated
                        mgr.setRatingStrategy(nullptr);
                        paginateShowRatings(entries, mgr);
                        mgr.saveRatingsToCSV("ratings.csv");
                        break;
                    }

                    case 2: {
                        // Manual rating for a specific match
                        clearScreen();
                        cout << "\nSelect match to rate:\n\n";
                        int matchIdx = paginateSelectMatches(entries);
                        if (matchIdx >= 0) {
                            double score;
                            cout << "\nEnter your score for match vs. "
                                << entries[matchIdx]->opponent
                                << " (1 to 10): ";
                            cin >> score;
                            mgr.setRatingStrategy(make_unique<UserRating>(score));
                            double s = mgr.ratePlayer(*entries[matchIdx]);
                            cout << "\nRecorded user score: " << fixed
                                << setprecision(2) << s << "\n";
                            mgr.saveRatingsToCSV("ratings.csv");
                            mgr.setRatingStrategy(nullptr);
                        }
                        else {
                            cout << "\nNo match selected.\n";
                        }
                        pauseForUser();
                        break;
                    }

                    case 3:
                        // Back to player selection
                        goto endPlayerActions;

                    default:
                        cout << "\nInvalid choice, try again.\n";
                        pauseForUser();
                        break;
                    } // end switch(choice)

                } // end while (per‐player actions)

            endPlayerActions:; // label to break out to player‐selection loop
            } // end while (select player)

            break; // back to top‐level menu
        }

        case 2: {
            // ─── Calculate average ratings for last X days ───
            while (true) {
                clearScreen();
                cout << "\nView average rating for:\n";
                cout << "1) A specific player\n";
                cout << "2) All players\n";
                cout << "3) Back to main menu\n";
                cout << "\nYour choice: ";
                int subChoice;
                if (!(cin >> subChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;  // invalid input → back to main menu
                }

                if (subChoice == 3) {
                    break;  // back to main menu
                }

                vector<string> nameList(uniqueNames.begin(), uniqueNames.end());
                string selectedName;
                bool singlePlayer = false;

                if (subChoice == 1) {
                    // Search + paginated list of players
                    clearScreen();
                    cout << "\n--- Search or Select Player (or 0 to go back) ---\n";
                    cout << "Enter a name (or part of one) to search: ";
                    string searchTerm;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, searchTerm);
                    if (searchTerm == "0") continue;

                    vector<string> filtered = searchPlayers(uniqueNames, searchTerm);
                    if (filtered.empty()) {
                        cout << "\nNo matching player found.\n";
                        pauseForUser();
                        continue;
                    }

                    int selIdx = paginateSelectStrings(filtered);
                    if (selIdx < 0) {
                        continue; // back to option 2 menu
                    }
                    selectedName = filtered[selIdx];
                    singlePlayer = true;
                }
                else if (subChoice == 2) {
                    singlePlayer = false; // show all
                }
                else {
                    cout << "\nInvalid choice, returning to main menu.\n";
                    pauseForUser();
                    break;
                }

                // Ask for lookback window
                clearScreen();
                cout << "\nEnter lookback window in days (0 = all matches): ";
                int days;
                if (!(cin >> days)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\nInvalid input; returning to main menu.\n";
                    pauseForUser();
                    break;
                }
                if (days < 0) {
                    cout << "\nNegative days not allowed; using all matches.\n";
                    days = 0;
                    pauseForUser();
                }

                // Display results
                clearScreen();
                if (singlePlayer) {
                    double avg = mgr.getAverageRatingLastDays(selectedName, days);
                    cout << "\nAverage rating for " << selectedName << " (";
                    if (days <= 0) cout << "All Time";
                    else           cout << "Last " << days << " Days";
                    cout << "): " << fixed << setprecision(2) << avg << "\n";
                }
                else {
                    cout << "\nAverage Ratings ";
                    if (days <= 0) cout << "(All Time)";
                    else           cout << "(Last " << days << " Days)";
                    cout << "\n----------------------------------------\n";
                    cout << left << setw(20) << "Player Name"
                        << setw(15) << "Avg. Rating\n";
                    cout << "----------------------------------------\n";
                    for (const auto& name : uniqueNames) {
                        double avg = mgr.getAverageRatingLastDays(name, days);
                        cout << left << setw(20) << name
                            << fixed << setprecision(2) << setw(15) << avg << "\n";
                    }
                    cout << "----------------------------------------\n";
                }

                pauseForUser();
                break;  // after showing results, go back to main menu
            } // end inner while for option 2

            break; // break out to main‐menu switch
        }

        case 3:
            // ─── Exit ───
            clearScreen();
            try {
                mgr.saveRatingsToCSV("ratings.csv");
                cout << "All ratings saved to ratings.csv\n";
            }
            catch (const exception& ex) {
                cerr << "Error saving ratings: " << ex.what() << "\n";
            }
            cout << "\nGoodbye.\n";
            return 0;

        default:
            cout << "\nInvalid choice, try again.\n";
            pauseForUser();
            break;
        } // end switch(topChoice)
    } // end while(true)

    return 0;
}
