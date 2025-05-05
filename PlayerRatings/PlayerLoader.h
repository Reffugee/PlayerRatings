#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H

#include "Player.h"
#include <vector>
#include <string>


std::vector<Player> loadPlayers(const std::string& filename);

#endif