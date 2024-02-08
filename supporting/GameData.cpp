#include "GameData.h"

/*
struct GameData {
    int currentHole;
    int currentStrokes;
};

class GameDataManager {
public:
    GameDataManager();
    void setHole(int hole);
    void setStrokes(int strokes);
    GameData getGameData();
private:
};

*/

GameDataManager::GameDataManager() {
    gameData.currentHole = 1;
    gameData.currentStrokes = 0;
}

void GameDataManager::setHole(int hole) {
    gameData.currentHole = hole;
}

int GameDataManager::getHole() const {
    return gameData.currentHole;
}

void GameDataManager::setStrokes(int strokes) {
    gameData.currentStrokes = strokes;
}

int GameDataManager::getStrokes() const {
    return gameData.currentStrokes;
}

GameData GameDataManager::getGameData() const {
    return gameData;
}

bool GameDataManager::canIncrementHole() {
    return gameData.currentHole < 18;
}

bool GameDataManager::canDecrementHole() {
    return gameData.currentHole > 1;
}

int GameDataManager::nextHole() {
    if (canIncrementHole()) {
        gameData.currentHole++;
    }
    return gameData.currentHole;
}

int GameDataManager::previousHole() {
    if (canDecrementHole()) {
        gameData.currentHole--;
    }
    return gameData.currentHole;
}

int GameDataManager::resetHole() {
    gameData.currentHole = 1;
    return gameData.currentHole;
}

void GameDataManager::resetStrokes() {
    gameData.currentStrokes = 0;
}

int GameDataManager::incrementStrokes() {
    gameData.currentStrokes++;
    return gameData.currentStrokes;
}

int GameDataManager::decrementStrokes() {
    if (gameData.currentStrokes > 0) {
        gameData.currentStrokes--;
    }
    return gameData.currentStrokes;
}
