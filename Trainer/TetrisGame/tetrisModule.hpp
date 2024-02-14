#include "../ai_instanceHandler.hpp"
#include "tetrisRules.hpp"
#include <random>
#include <iostream>

void runTetrisGame(AI_InstanceHandler weightConfigHandler, int maxPieces, int (&results)[4], std::mt19937 &generator);