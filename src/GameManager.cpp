#include "../include/GameManager.h"
#include "view/View.h"
#include "../include/logic/Point.h"
#include <vector>

GameManager::GameManager() {}

GameManager::~GameManager() {}

void GameManager::startGame() {
    View view;

    // TODO(real data): load level points and midRowCount from the real data source.
    const std::vector<Point> levelPoints;
    const int                midRowCount = 5;

    view.run(levelPoints, midRowCount);
}
