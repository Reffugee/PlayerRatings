#include <iostream>
#include "Player.h"
#include "Stats.h"
#include "AutomatedRating.h"
#include "UserRating.h"
#include "RatingManager.h"

using namespace std;

int main() {
    cout << "--- Football Rating App ---\n";

    Stats stats(
        1,  // goals
        0,  // assists
        5,  // tackles
        2,  // key passes
        6,  // successful dribbles
        7,  // bad passes
        2,  // fouls
        1,  // offsides
        1,  // yellow cards
        0,  // red cards
        2,  // dispossessions
        1,  // errors leading to goal
        0   // own goals
    );

    Player player("Leo Tactics", "Attacking Midfielder", stats);

    AutomatedRating autoRating;
    RatingManager manager;

    int choice = 0;
    while (true) {
        cout << "\n--- Rating Menu ---" << endl;
        cout << "1. Evaluarea automata a jucatorului" << endl;
        cout << "2. Evaluare jucator" << endl;
        cout << "3. Exit" << endl;
        cout << "Alege optiunea: ";
        cin >> choice;

        if (choice == 1) {
            manager.setRatingStrategy(&autoRating);
            cout << "Evaluare automata: " << manager.ratePlayer(player) << endl;
        }
        else if (choice == 2) {
            double userScore;
            cout << "Introdu nota pentru jucator (1-10): ";
            cin >> userScore;
            UserRating userRating(userScore);
            manager.setRatingStrategy(&userRating);
            cout << "Nota utilizator: " << manager.ratePlayer(player) << endl;
        }
        else if (choice == 3) {
            cout << "Inchidere program." << endl;
            break;
        }
        else {
            cout << "Optiune invalida. Incearca din nou." << endl;
        }
    }

    return 0;
}
