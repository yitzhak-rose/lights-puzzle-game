#include "game/GameManager.h"
#include "view/View.h"

GameManager::GameManager() {}

GameManager::~GameManager() {}

void GameManager::startGame() {
	View view;
	view.run();
}
